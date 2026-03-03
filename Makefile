#
# Makefile for the "grow" DPF plugin
#

# 1. Plugin-specific settings
NAME := grow
FILES_DSP := grow.cpp
FILES_UI := UIGrow.cpp

# 2. Plugin formats to build
TARGETS := vst3

ifeq ($(shell uname -s),Darwin)
    TARGETS += au
endif

# 3. Universal DaisySP Integration
DAISYSP_DIR = $(abspath ../../lib/DaisySP/Source)
DAISYSP_LGPL_DIR = $(abspath ../../lib/DaisySP/DaisySP-LGPL/Source)

# 4. DGL and UI Include Paths
DPF_DIR = $(abspath ../../dpf)
DGL_DIR = $(DPF_DIR)/dgl

# Add standard DGL path
CXXFLAGS += -I$(DGL_DIR)
BUILD_CXX_FLAGS += -I$(DGL_DIR)

# (Optional) If you also need to explicitly target NanoVG's source folder for specific includes
CXXFLAGS += -I$(DGL_DIR)/src/nanovg
BUILD_CXX_FLAGS += -I$(DGL_DIR)/src/nanovg

# Enable NanoVG support for the UI
CXXFLAGS += -DDISTRHO_UI_USE_NANOVG=1
BUILD_CXX_FLAGS += -DDISTRHO_UI_USE_NANOVG=1

# DGL web view support (must match DGL build configuration)
CXXFLAGS += -DDGL_USE_WEB_VIEW
BUILD_CXX_FLAGS += -DDGL_USE_WEB_VIEW

# Define all subdirectories
DAISYSP_SUBDIRS = Control Dynamics Effects Filters PhysicalModeling Synthesis Utility

# Add all subdirectories to the include paths so headers resolve automatically
CXXFLAGS += -isystem $(DAISYSP_DIR) -isystem $(DAISYSP_LGPL_DIR)
BUILD_CXX_FLAGS += -isystem $(DAISYSP_DIR) -isystem $(DAISYSP_LGPL_DIR)

$(foreach dir, $(DAISYSP_SUBDIRS), \
    $(eval CXXFLAGS += -isystem $(DAISYSP_DIR)/$(dir)) \
    $(eval BUILD_CXX_FLAGS += -isystem $(DAISYSP_DIR)/$(dir)) \
    $(eval CXXFLAGS += -isystem $(DAISYSP_LGPL_DIR)/$(dir)) \
    $(eval BUILD_CXX_FLAGS += -isystem $(DAISYSP_LGPL_DIR)/$(dir)) \
)

# Enable LGPL macro
CXXFLAGS += -DUSE_DAISYSP_LGPL

# Add All DaisySP Source Files via Wildcard
$(foreach dir, $(DAISYSP_SUBDIRS), \
    $(eval FILES_DSP += $(wildcard $(DAISYSP_DIR)/$(dir)/*.cpp)) \
    $(eval FILES_DSP += $(wildcard $(DAISYSP_LGPL_DIR)/$(dir)/*.cpp)) \
)

# 5. DPF Build System
include ../../dpf/Makefile.plugins.mk

# Force the targets rule
all: $(TARGETS)