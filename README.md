# Examen final de teoría de compiladores (Pregunta 3) 
* Alumno: Esteban Cabrera U202014600
* Profesor: Luis Canaval
## Enunciado
Tomando como base el código de su TF y los ejemplos de clase, construya un pequeño lenguaje de programación que permito el uso de estructuras condicionales IF y haga uso de expresiones en notación prefijo. Ud. puede definir sus palabras reservadas y al menos 4 operadores.
* (5 puntos) analizador sintáctico y léxico (opcionalmente usando ANTLR4).
* (5 puntos) generación de código IR.
* (5 puntos) optimización de código.
* (3 puntos) ejecución en tiempo real (como ejemplo fibonacci). 

## Instalación de los recursos necesarios
Se instalo las librerías de antrl4 siguiendo estos pasos:
* Se instala el antlr4 por medio de paquetería de un archivo .jar: https://www.antlr.org/download.html
* Se instala el antrl4 runtime para cpp siguiendo los pasos similares a este enlace: https://github.com/antlr/antlr4/tree/dev/runtime/Cpp
```
cd <antlr4-dir cpp> (this is where this readme is located)
mkdir build && mkdir run && cd build
cmake .. -DANTLR_JAR_LOCATION=full/path/to/antlr4.jar -DWITH_DEMO=True
make
DESTDIR=<antlr4-dir cpp>/run make install
```
* Se crea un script de bashrc para almacenar los paths y así ahorrar tiempo en ejecutar comandos extensos de antlr4
```
export MYDEPENDENCIES=<ruta del archivo jar de antrl4>
export CLASSPATH=.:$MYDEPENDENCIES/antlr-4.13.1-complete.jar:$CLASSPATH
alias antlr4='java org.antlr.v4.Tool'
alias grun='java org.antlr.v4.gui.TestRig'

export ANTLR4HOME=$MYDEPENDENCIES/antlr4-cpp-runtime-4.13.1-source/run
export ANTLR4I=$ANTLR4HOME/usr/local/include/antlr4-runtime
export ANTLR4L=$ANTLR4HOME/usr/local/lib/
```
Para la instalación de LLVM se uso estos comandos para linux Ubuntu/debian:
* Clang and co
`sudo apt-get install clang-15 clang-tools-15 clang-15-doc libclang-common-15-dev libclang-15-dev libclang1-15 clang-format-15 python3-clang-15 clangd-15 clang-tidy-15`
* LLVM
`sudo apt-get install libllvm-15-ocaml-dev libllvm15 llvm-15 llvm-15-dev llvm-15-doc llvm-15-examples llvm-15-runtime`
* LLBD
`sudo apt-get install lldb-15`
* Luego en el script se añade estas rutas:
```
export LLVMMYDEPENDENCIES="/usr"
export LLVM15I=$LLVMMYDEPENDENCIES/include/llvm-15
export LLVM15IC=$LLVMMYDEPENDENCIES/include/llvm-c-15
export LLVM15L=$LLVMMYDEPENDENCIES/lib/llvm-15
```
Para comprobar que todo está correctamente instalado, se ejecutan estos comandos:
```
antlr4
llvm-15-dev --version
```

## Procedimiento
* Se creo un archivo antrl4 con la grámatica solicitada, esto es para generar un analizador léxico y sintáctico de forma rápida. Usando estos comandos para generar los tokens, parser y visitor
```
antlr4 -no-listener -visitor ExprParser.g4
```
* Se creo dos visitors que van a manipular los nodos del árbol AST que se genera de nuestra grámatica al igual que el generador de código IR de LLVM
* Todo ese procesor se va a juntar en nuestro archivo main.cpp
* En ello, ejecutamos nuestro proyecto usando el CMakeList del repositorio con este comando:
```
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1
make --build -j8
```
* De esta forma podemos ejecutar nuestro archivo generado dentro de la carpeta build llamada **prog** teniendo como entrado nuestro archivo **simple.k**:
```
./build/prog < simple.k
```   

  
  
