# Author: mosra <mosra@centrum.cz>
pkgname=kompas-plugins-nonfree
pkgver=dev
pkgrel=1
pkgdesc="Non-free plugins for Kompas navigation software"
arch=('i686' 'x86_64')
url="http://mosra.cz/blog/kompas.php"
license=('LGPLv3')
makedepends=('cmake' 'qt')
depends=('kompas-core')
options=(!strip)

build() {
    mkdir -p "$startdir/build"
    cd "$startdir/build"

    cmake .. \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DBUILD_TESTS=TRUE
    make
}

check() {
    cd "$startdir/build"
    ctest
}

package() {
  cd "$startdir/build"
  make DESTDIR="$pkgdir/" install
}
