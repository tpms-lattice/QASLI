# QASLI
QASLI is a graphical user interface (GUI) for [ASLI](https://github.com/tpms-lattice/ASLI) lattice infill code. QASLI is written in Qt and C++ and runs on all major desktop platforms (Linux, Windows, and macOS).

## Building and installing QASLI

In order to use QASLI, you need to build it from the source codes. Since QASLI is developed using Qt, compiling the source files is quite straightforward. Upon installing Qt on your machine (you can find the installation instruction for various platforms [here](https://doc.qt.io/qt-5/gettingstarted.html)), clone this repository (`git clone https://github.com/tpms-lattice/QASLI.git`) and follow one of the following scenarios to build QASLI. It should be noted that prior to running QASLI, you need to build [ASLI](https://github.com/tpms-lattice/ASLI) and put its executable in the source directory.

### Build QASLI using Qt Creator IDE

This is the simplest technique to build the program, and it has a similar procedure for all the supported platforms. Qt Creator is the default IDE for Qt development, so it is automatically installed along with Qt. Simply open the project CMake file (`CMakeLists.txt`) in Qt Creator (by executing `qtcreator CMakeLists.txt` or selecting *File->Open Project* from the IDE) and build the project (by pressing *Shift+B*).

### Build QASLI using Qt tools in Linux

Building the source files in Linux is quite simple. Navigate to the source files directory (the cloned repository) and run the following commands (this assumes that you have already added Qt `bin` directory to the `PATH` variable so that the CMake script can find Qt libraries and binaries):

  ```Shell
  mkdir build
  cd build
  cmake ..
  make
  ```

After doing this, you can find the `QASLI` executable in the build directory and run it by `./QASLI`.

### Build QASLI using Qt tools in Windows

Although building QASLI in Windows follows a procedure similar to Linux, it contains more practicalities in action. Since Qt doesn't add its binaries folder to the `PATH` environment variable, you need to use the "Qt command window". Open the "Qt command window" (the name is something like `Qt 5.15.2 (MinGW 8.1.0 64-bit)` -with different version numbers and compiler name matching your Qt installation- in the Start menu). Then, navigate to the source files directory (the cloned repository) and run the following commands (assuming you have installed the MinGW integration for Qt):

  ```Shell
  mkdir build
  cd build
  cmake -G "MinGW Makefiles" ..
  mingw32-make
  ```

After doing this, you can find the `QASLI` executable in the build directory, and the best way to run it is by executing `.\QASLI.exe` via the "Qt command window".

## Issues and questions ##
Please post an [issue](https://github.com/tpms-lattice/QASLI/issues) for any bug/suggestion/question you may find/have.

## License ##
QASLI is licensed under the [terms of the GNU General Public License](https://www.gnu.org/licenses/).
