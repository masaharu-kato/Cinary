# CMakeList.txt : 最上位の CMake プロジェクト ファイル。グローバル構成を行います
# また、サブプロジェクトをここに含めます。
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project("sample_in")
project("sample_out")
project("sample_any")
project("trial")

# サブプロジェクトを含めます。
add_subdirectory ("src")
add_subdirectory ("trial")