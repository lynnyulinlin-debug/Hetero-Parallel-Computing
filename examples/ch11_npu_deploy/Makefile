# ch11_npu_deploy Makefile
# 目标：一键跑通导出、量化、转换和推理脚手架。

PYTHON ?= python3
SHELL  ?= /bin/bash

MODEL       ?= resnet18
BACKEND     ?= rknn
ARTIFACTS   ?= artifacts
CALIB_DIR   ?= data/calib
CALIB_LIST  ?= data/calib/list.txt
TEST_DIR    ?= data/test
TEST_LIST   ?= data/test/list.txt

ONNX_OUT    ?= $(ARTIFACTS)/model.onnx
EXPORT_RPT  ?= $(ARTIFACTS)/export_report.json
QUANT_OUT   ?= $(ARTIFACTS)/model.quant
QUANT_RPT   ?= $(ARTIFACTS)/quant_report.json
INFER_TXT   ?= $(ARTIFACTS)/infer_report.txt
INFER_JSON  ?= $(ARTIFACTS)/infer_report.json

.PHONY: help export quantize convert-rknn convert-ascend infer run clean

help:
	@printf '%s\n' \
		'Targets:' \
		'  make export          # export model to ONNX manifest' \
		'  make quantize        # generate quantization manifest' \
		'  make convert-rknn    # generate RKNN conversion artifact' \
		'  make convert-ascend  # generate Ascend conversion artifact' \
		'  make infer           # generate inference report' \
		'  make run             # export -> quantize -> infer' \
		'  make clean           # remove artifacts and caches'

export:
	$(PYTHON) 01_export_onnx.py \
		--model $(MODEL) \
		--output $(ONNX_OUT) \
		--report $(EXPORT_RPT)

quantize: export
	$(PYTHON) 02_quantize.py \
		--backend $(BACKEND) \
		--onnx $(ONNX_OUT) \
		--calib-dir $(CALIB_DIR) \
		--calib-list $(CALIB_LIST) \
		--output $(QUANT_OUT) \
		--report $(QUANT_RPT)

convert-rknn: quantize
	$(PYTHON) 03_convert_rknn.py --onnx $(ONNX_OUT) --output $(ARTIFACTS)/model.rknn

convert-ascend: quantize
	$(SHELL) 03_convert_ascend.sh $(ONNX_OUT) $(ARTIFACTS)/model.om

infer: quantize
	$(PYTHON) 04_infer.py \
		--backend $(BACKEND) \
		--model $(QUANT_OUT) \
		--test-dir $(TEST_DIR) \
		--test-list $(TEST_LIST) \
		--output $(INFER_TXT) \
		--report-json $(INFER_JSON)

run: infer

clean:
	rm -rf $(ARTIFACTS) __pycache__ stages/__pycache__
