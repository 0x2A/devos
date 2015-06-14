**This is a hobby OS project for learning purposes and might never become a usable OS.**



### Hardware Requirements:

- x86 CPU (i686 or higher)
- minimum 10 mb ram
- VGA compatible GPU

### setup and compile

##### Windows
on windows you need [cygwin](http://www.cygwin.com/) to compile with the following packets

  - **grub**
  - bison
  - flex
  - gcc4
  - make
  - nasm
  - wget

#### build cross-compiler

run these commands in console:
```
export PREFIX=/usr/local/cross
export TARGET=i686-elf
export PATH=$PATH:$PREFIX/bin
mkdir cross_gcc
cd cross_gcc
mkdir build_binutils build_gcc
```

Download and unpack source code:
```
wget http://ftp.gnu.org/gnu/binutils/binutils-2.25.tar.bz2
wget http://ftp.gnu.org/gnu/gcc/gcc-5.1.0/gcc-5.1.0.tar.bz2
wget http://ftp.gnu.org/gnu/gmp/gmp-4.3.2.tar.bz2
wget http://www.mpfr.org/mpfr-2.4.2/mpfr-2.4.2.tar.bz2
wget http://www.multiprecision.org/mpc/download/mpc-0.8.1.tar.gz
tar xjf binutils-2.25.tar.bz2
tar xjf gcc-5.1.0.tar.bz2
tar xjf gmp-4.3.2.tar.bz2
tar xjf mpfr-2.4.2.tar.bz2
tar xzf mpc-0.8.1.tar.gz
mv gmp-4.3.2 gcc-5.1.0/gmp
mv mpfr-2.4.2 gcc-5.1.0/mpfr
mv mpc-0.8.1 gcc-5.1.0/mpc
```
*Hint for windows: cygwin doesnt provide wget, so download the files with your browser and copy them to the target directory*

Build binutils:
```
cd build_binutils
../binutils-2.25/configure --target=$TARGET --prefix=$PREFIX --disable-nls
make all
make install
cd ..
```

Build gcc:
```
cd build_gcc
../gcc-5.1.0/configure --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c --without-headers
make all-gcc
make install-gcc
```

#### compile and run
```
cd devos
make
./createBootDisk.sh
```
This will create an iso file called devos.iso in build directory. Use any virtual machine you like to boot from the iso or burn and mount on your real hardware (NO WARRANTY OF ANY KIND!)

### Info:

Feel free to fork or commit. If you have any problems or suggestions feel free to open a ticket.
