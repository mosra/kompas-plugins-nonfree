Name: kompas-plugins-nonfree
URL: http://mosra.cz/blog/kompas.php
Version: 0.1.1
Release: 1
License: LGPLv3
Group: Productivity/Graphics/Viewers
Source: https://github.com/mosra/%{name}/tarball/v%{version}/%{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
BuildRequires: cmake >= 2.6.0
BuildRequires: kompas-core-devel = %{version}
#BuildRequires: kompas-qt-devel = %{version}
BuildRequires: kompas-plugins-devel = %{version}

Summary: Non-free plugins for Kompas navigation software
Requires: kompas-core = %{version}
#Requires: kompas-qt = %{version}
Requires: kompas-plugins = %{version}

%description
Plugins for viewing content under restrictive licenses, like for example
Google Maps.

%package devel
Group: Development/Libraries/C and C++
Summary: Kompas Non-free plugins development files
Requires: %{name} = %{version}
Requires: kompas-core-devel = %{version}
#Requires: kompas-qt-devel = %{version}
Requires: kompas-plugins-devel = %{version}

%description devel
Non-free plugins headers needed for developing for Kompas.

# TODO: Split future Qt plugins to package kompas-plugins-nonfree-qt ?

%prep
%setup -q -n mosra-kompas-plugins-nonfree-c922d69

%build
export CFLAGS="$RPM_OPT_FLAGS"
mkdir build && cd build
cmake .. \
    -DCMAKE_INSTALL_PREFIX=%{_prefix} \
    -DCMAKE_BUILD_TYPE=Release
make %{?_smp_mflags}

%install
cd build
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{_prefix}
%exclude %{_prefix}/include
%exclude %{_prefix}/share/*/Modules

%files devel
%defattr(-,root,root,-)
%{_prefix}/include/Kompas
%{_prefix}/share/*/Modules
