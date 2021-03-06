#!/bin/bash
set -e

export LD_LIBRARY_PATH=.:`cat /etc/ld.so.conf.d/* | grep -v -E "#" | tr "\\n" ":" | sed -e "s/:$//g"`
sudo add-apt-repository -y ppa:kevinkreiser/prime-server
sudo apt-get update -o Dir::Etc::sourcelist="sources.list.d/kevinkreiser-prime-server-$(lsb_release -c -s).list" -o Dir::Etc::sourceparts="-" -o APT::Get::List-Cleanup="0"
sudo apt-get install -y autoconf automake pkg-config libtool make pkg-config cmake pkg-config gcc g++ lcov libboost1.54-all-dev protobuf-compiler libprotobuf-dev libprime-server-dev

#clone async
mkdir -p deps
for dep in midgard baldr; do
	rm -rf $dep
	git clone --depth=1 --recurse-submodules --single-branch --branch=master https://github.com/valhalla/$dep.git deps/$dep &
done
wait

#install locales
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
$DIR/install_locales.sh $(grep -F posix_locale $DIR/../locales/*.json | sed -e "s/.*locale[^a-z^A-Z]\+//g" -e "s/[^a-z^A-Z^0-9^.^_]\+//g")

#build sync
for dep in midgard baldr; do
	pushd deps/$dep
	./autogen.sh
	./configure CPPFLAGS="-DBOOST_SPIRIT_THREADSAFE -DBOOST_NO_CXX11_SCOPED_ENUMS"
	make -j$(nproc)
	sudo make install
	popd
done
