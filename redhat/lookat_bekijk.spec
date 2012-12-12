Name:		lookat_bekijk
Version:	1.4.2stable1
Release:	1%{?dist}
Summary:	Lookat / bekijk is a program to view Un*x text files and man pages. 

Group:		Applications/Text
License:	GPLv2+	
URL:		http://www.wagemakers.be/english/programs/lookat
Source0:	lookat_bekijk-%{version}.tar.gz
BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:	gcc, ncurses-devel
Requires:	ncurses-libs	

%description
"lookat"  (or  "bekijk" in the Dutch version) is a program to view Un*x text files and manual pages.

Under DOS I used list.com to view text files. I didn't find such a program under my favorite OS, GNU/Linux. The standard Un*x utilities ( more, less, view ...) weren't userfriendly enough. For this reason I created "lookat".

%prep
%setup -q
./configure --prefix=/usr --sysconfdir=/etc --mandir=/usr/share/man



%build
%configure
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
/usr/bin/lookat
/usr/bin/bekijk
%doc /usr/share/doc/lookat/README
%doc /usr/share/doc/lookat/examples/blue_config.cfg
%doc /usr/share/man/man1/bekijk.1.gz
%doc /usr/share/man/man1/lookat.1.gz
%doc /usr/share/doc/lookat/LEESMIJ
%config /etc/lookat.conf
%config /etc/lookat.conf.default





%changelog

