@ECHO OFF


IF EXIST %1 (
	lexer %1
	start /wait cmd /K semantic "output.mlx"
)
ELSE (
	ECHO.
	ECHO THE MOE PROGRAMMING LANGUAGE.
	ECHO Type the complete path of the *.moe file to be parsed and press Enter, then F6 (or Ctrl+Z), then Enter.
	ECHO Only the last non-empty line will be remembered, leading spaces are ignored.
	ECHO.

	:: Only one single command line is needed to receive user input
	FOR /F "tokens=*" %%A IN ('TYPE CON') DO SET INPUT=%%A

	:: Use quotes if you want to display redirection characters as well
	start /wait cmd /K lexer "%INPUT%"
	start /wait cmd /K semantic "output.mlx"
)