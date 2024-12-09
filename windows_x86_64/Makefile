SHELL := /bin/bash

CUR_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
ROOT_DIR := $(CUR_DIR)/../

$(ROOT_DIR)/venv/bin/ansible-playbook:
	python3 -m venv $(ROOT_DIR)/venv
	$(ROOT_DIR)/venv/bin/python -m pip install ansible==7.1.0 pywinrm

ansible: $(ROOT_DIR)/venv/bin/ansible-playbook

repair:
	cp /root/.local/share/libvirt/images/* /var/lib/libvirt/images 
	rm -rf /root/.local/share/libvirt/images/*
	virsh pool-define-as --name kafl_pool --type dir --target /var/lib/libvirt/images
	virsh pool-autostart kafl_pool
	virsh pool-start kafl_pool

init:
	vagrant up --no-provision
	vagrant snapshot save 'ready_provision'

compile:
	$(MAKE) -C src/NtExecutor all

provision_userspace: compile ansible
	vagrant snapshot restore 'ready_provision'
	TARGET_HARNESS='userspace' vagrant provision
	vagrant halt

provision_driver: compile ansible
	vagrant snapshot restore 'ready_provision'
	TARGET_HARNESS='driver' vagrant provision
	vagrant halt
