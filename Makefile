SHELL := /bin/bash

TESTS :=
OPTIONS :=

PACKAGE_NAME := $(shell cat package.txt)
PACKAGE_VERSION := $(shell cat version.txt)

BUILD_TYPE := Release
PYTHON_VERSION := 3.12.7
VENV_NAME := $(shell cat .python-version)

ENABLE_TESTING := ON
ENABLE_COVERAGE := OFF

CONAN_OPTIONS := -s:b compiler.cppstd=20 -s:h compiler.cppstd=20 --profile:build=default --profile:host=default --build=missing
PIP_OPTIONS :=

.PHONY: all
all: build

.PHONY: clean
clean:
	make -C build/${BUILD_TYPE} clean

.PHONY: very-clean
very-clean:
	rm -fr ./build ./test_package/build

.PHONY: configure
configure:
	mkdir -p build; cd build; conan install .. ${CONAN_OPTIONS} -o '&:ENABLE_COVERAGE=${ENABLE_COVERAGE}' -o '&:ENABLE_TESTING=${ENABLE_TESTING}'

.PHONY: build
build: configure
	cd build; conan build .. ${CONAN_OPTIONS} -o '&:ENABLE_COVERAGE=${ENABLE_COVERAGE}' -o '&:ENABLE_TESTING=${ENABLE_TESTING}'

.PHONY: tests
tests:
	source build/${BUILD_TYPE}/generators/conanrun.sh; cd tests; ../build/${BUILD_TYPE}/tests/tests ${TESTS} --skip-benchmarks

.PHONY: benchmark-quick
benchmark-quick:
		source build/${BUILD_TYPE}/generators/conanrun.sh; cd tests; ../build/${BUILD_TYPE}/tests/tests --benchmark-samples 10

.PHONY: coverage
coverage:
	@gcovr --version
	@mkdir -p coverage
	cd build; gcovr --delete --print-summary --merge-mode-functions separate --exclude-unreachable-branches --html --html-details -o ../coverage/index.html --xml-pretty --xml ../coverage/coverage.xml --filter '../src/' --filter '../include/' --exclude '../test/'

.PHONY: conan-package
conan-package:
	export CONAN_REVISIONS_ENABLED=1; export LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH; conan create . ${CONAN_OPTIONS} --build-require #--format=json

clang-format:
	clang-format --verbose -i $(shell find demo include src tests test_package -name '*pp' -not -path 'test_package/build/*') --style=LLVM

.PHONY: pyenv pyenv-download pyenv-python pyenv-venv
pyenv: pyenv-download pyenv-python pyenv-venv

pyenv-download:
	curl -L https://github.com/pyenv/pyenv-installer/raw/master/bin/pyenv-installer | bash

pyenv-python:
	export PYTHON_CONFIGURE_OPTS="--enable-shared"; ~/.pyenv/bin/pyenv install --force $(PYTHON_VERSION)

pyenv-venv:
	~/.pyenv/bin/pyenv virtualenv $(PYTHON_VERSION) $(VENV_NAME)

.PHONY: pyenv-setup
pyenv-setup: pyenv-conan pyenv-tests

.PHONY: pyenv-conan
pyenv-conan:
	python3 -m pip install $(PIP_OPTIONS) --upgrade pip
	pip3 install $(PIP_OPTIONS) conan --upgrade

.PHONY: pyenv-tests
pyenv-tests:
	pip3 install $(PIP_OPTIONS) gcovr

.PHONY: codechecker
codechecker:
	rm -fr codechecker
	CodeChecker analyze build/Release/compile_commands.json -o ./codechecker || true
	CodeChecker parse ./codechecker -e html -o ./codechecker || true

.PHONY: run-demo
run-demo:
	source build/${BUILD_TYPE}/generators/conanrun.sh; build/${BUILD_TYPE}/demo/demo
