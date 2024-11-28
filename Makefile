# Makefile 示例

# 设置 CMake 构建目录
BUILD_DIR  = build

# 设置 CMake 命令
CMK = cmake

# 设置 CMake 配置命令
CMK_CONFIG = $(CMK) -G Ninja -S . -B $(BUILD_DIR)

# 设置 CMake 构建命令
CMK_BUILD = ninja -C build

# 设置清理命令
CLEAN = rm -rf $(BUILD_DIR)

# 默认目标
all: config build

# 配置目标
config:
	@echo "Configuring project..."
	@mkdir -p $(BUILD_DIR)
	@$(CMK_CONFIG)

# 构建目标
build:
	@echo "Building project..."
	@$(CMK_BUILD)

# 清理目标
clean:
	@echo "Cleaning up..."
	@$(CLEAN)

# 重新配置并构建
replay: clean config build

play: config build

runmain:
	@./build/main ./test/ sorted.txt

.PHONY: all config build clean rebuild