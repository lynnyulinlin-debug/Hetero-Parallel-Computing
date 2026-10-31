#!/usr/bin/env python3
"""Validate that teaching deployment reports cannot be mistaken for device measurements."""

from __future__ import annotations

import json
import subprocess
import sys
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PROJECTS = (
    ("ch11_classification_infer", {"top1_acc", "top5_acc"}),
    ("ch11_npu_deploy", {"accuracy"}),
)
BASE_FIELDS = {"schema_version", "execution_mode", "metric_source", "backend", "latency_ms"}


def main() -> None:
    with tempfile.TemporaryDirectory() as temporary:
        temp = Path(temporary)
        for project, metric_fields in PROJECTS:
            script = ROOT / "examples" / project / "04_infer.py"
            report = temp / f"{project}.json"
            result = subprocess.run(
                [sys.executable, str(script), "--report-json", str(report), "--output", str(temp / f"{project}.txt")],
                cwd=script.parent,
                text=True,
                capture_output=True,
            )
            if result.returncode != 0:
                raise RuntimeError(result.stderr)
            payload = json.loads(report.read_text(encoding="utf-8"))
            if BASE_FIELDS - payload.keys() or metric_fields - payload.keys():
                raise AssertionError(f"{project}: incomplete report schema")
            if payload["execution_mode"] != "simulated" or payload["metric_source"] != "teaching_profile":
                raise AssertionError(f"{project}: simulated provenance missing")
            real = subprocess.run([sys.executable, str(script), "--mode", "real"], cwd=script.parent, text=True, capture_output=True)
            if real.returncode == 0:
                raise AssertionError(f"{project}: real mode accepted missing prerequisites")

        pipeline = ROOT / "examples" / "ch14_face_detection" / "pipeline.py"
        simulated_report = temp / "face-simulated.json"
        simulated = subprocess.run(
            [sys.executable, str(pipeline), "--frames", "2", "--report-json", str(simulated_report)],
            cwd=pipeline.parent, text=True, capture_output=True,
        )
        if simulated.returncode != 0:
            raise RuntimeError(simulated.stderr)
        payload = json.loads(simulated_report.read_text(encoding="utf-8"))
        if payload.get("execution_mode") != "simulated" or payload.get("metric_source") != "teaching_pipeline":
            raise AssertionError("face pipeline: simulated provenance missing")

        real_input = temp / "jetson-runtime.json"
        real_input.write_text(json.dumps({
            "execution_mode": "real", "experiment_config_id": "jetson-orin-nano-4gb-video-baseline-v1",
            "platform": "Jetson Orin Nano", "runtime_version": "test",
            "model": "face.engine", "input_source": "video.mp4", "frames": 2,
            "input_kind": "video_file", "input_sha256": "0" * 64,
            "input_width": 1280, "input_height": 720, "warmup_frames": 10, "measured_frames": 2,
            "memory_peak_mb": 2048, "swap_peak_mb": 0,
            "fps": 20.0, "latency_ms_p50": 40.0, "latency_ms_p95": 48.0,
        }), encoding="utf-8")
        real_output = temp / "jetson-archived.json"
        real = subprocess.run(
            [sys.executable, str(pipeline), "--mode", "real", "--real-report", str(real_input), "--report-json", str(real_output)],
            cwd=pipeline.parent, text=True, capture_output=True,
        )
        if real.returncode != 0 or json.loads(real_output.read_text(encoding="utf-8"))["platform"] != "Jetson Orin Nano":
            raise AssertionError("face pipeline: real report contract failed")

        invalid_input = temp / "camera-runtime.json"
        invalid = json.loads(real_input.read_text(encoding="utf-8"))
        invalid["input_kind"] = "camera"
        invalid_input.write_text(json.dumps(invalid), encoding="utf-8")
        rejected = subprocess.run(
            [sys.executable, str(pipeline), "--mode", "real", "--real-report", str(invalid_input)],
            cwd=pipeline.parent, text=True, capture_output=True,
        )
        if rejected.returncode == 0 or "video-baseline requires" not in rejected.stderr:
            raise AssertionError("face pipeline: live input bypassed the video baseline")

        invalid_hash = temp / "invalid-hash-runtime.json"
        invalid["input_kind"] = "video_file"
        invalid["input_sha256"] = "z" * 64
        invalid_hash.write_text(json.dumps(invalid), encoding="utf-8")
        rejected = subprocess.run(
            [sys.executable, str(pipeline), "--mode", "real", "--real-report", str(invalid_hash)],
            cwd=pipeline.parent, text=True, capture_output=True,
        )
        if rejected.returncode == 0 or "input_sha256" not in rejected.stderr:
            raise AssertionError("face pipeline: invalid input hash was accepted")


if __name__ == "__main__":
    main()
