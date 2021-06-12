del /s *.sln
del /s *.vcxproj
del /s *.vcxproj.filters
del /s *.vcxproj.user
rmdir /Q /S .vs
rmdir /Q /S out
rmdir /Q /S Dependencies\GLEW\out
rmdir /Q /S Dependencies\GLFW\out
rmdir /Q /S Dependencies\imGUI\out
rmdir /Q /S Dependencies\spdlog\out
rmdir /Q /S out