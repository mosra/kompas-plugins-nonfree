Name: kompas-plugins-nonfree
URL: http://mosra.cz/blog/kompas.php
Version: 0.1.2
Release: 1
License: LGPLv3
%if %{defined suse_version}
Group: Productivity/Graphics/Viewers
%else
Group: Applications/Multimedia
%endif
Source: https://github.com/mosra/%{name}/tarball/v%{version}/%{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
BuildRequires: gcc-c++
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
%if %{defined suse_version}
Group: Development/Libraries/C and C++
%else
Group: Development/Libraries
%endif
Summary: Kompas Non-free plugins development files
Requires: %{name} = %{version}
Requires: kompas-core-devel = %{version}
#Requires: kompas-qt-devel = %{version}
Requires: kompas-plugins-devel = %{version}

%description devel
Non-free plugins headers needed for developing for Kompas.

# TODO: Split future Qt plugins to package kompas-plugins-nonfree-qt ?

%prep
%setup -q -n mosra-kompas-plugins-nonfree-fca0b11

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
strip $RPM_BUILD_ROOT/%{_prefix}/lib*/kompas/*/*.so

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{_prefix}/lib*/kompas
%doc COPYING COPYING.LESSER

%files devel
%defattr(-,root,root,-)
%{_prefix}/include/Kompas
%{_prefix}/share/*/Modules
%doc COPYING COPYING.LESSER

%changelog
* Sun Mar 20 2011 Vladimír Vondruš <mosra@centrum.cz> - 0.1.2-1
- New upstream version 0.1.2.

* Sat Feb 26 2011 Vladimír Vondruš <mosra@centrum.cz> - 0.1.1-1
- Initial release.
