# CMakeList.txt : cinary の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#
cmake_minimum_required (VERSION 3.8)

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (cinary_in  "cinary_in.cpp"  "cinary.h" "bstream.h")
add_executable (cinary_out "cinary_out.cpp" "cinary.h" "bstream.h")

# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。
