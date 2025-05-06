# make sure this is executed with bash
SHELL := /bin/bash

YELLOW := "\e[1;33m"
NC := "\e[0m"

# Logger function
INFO := @bash -c '\
  printf $(YELLOW); \
  echo "=> $$1"; \
  printf $(NC)' SOME_VALUE

VENV_DIR := .venv
PYTHON := python3.10
PIP := $(VENV_DIR)/bin/pip
VENV_PY := $(VENV_DIR)/bin/python

.DEFAULT_GOAL := help

$(VENV_DIR):
	$(INFO) "Creating virtual environment..."
	$(PYTHON) -m venv $(VENV_DIR)

$(PIP): $(VENV_DIR)
	$(INFO) "Installing base tooling: pip, wheel, setuptools..."
	$(PIP) install -U pip setuptools wheel twine

install: $(PIP)
	$(INFO) "Installing requirements..."
	$(VENV_PY) -m pip install -r requirements.txt

install_precommit: $(PIP)
	$(INFO) "Installing pre-commit hooks..."
	$(VENV_PY) -m pip install pre-commit
	$(VENV_DIR)/bin/pre-commit install