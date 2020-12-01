FROM registry.fedoraproject.org/fedora:33

RUN dnf update -y && \
    dnf install -y \
        autoconf \
        automake \
        ca-certificates \
        ccache \
        gcc \
        git \
        glibc-langpack-en \
        libvirt-devel \
        make \
        net-snmp-devel \
        perl-Net-SNMP \
        pkgconfig \
        rpm-build && \
    dnf autoremove -y && \
    dnf clean all -y && \
    mkdir -p /usr/libexec/ccache-wrappers && \
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/cc && \
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/$(basename /usr/bin/gcc)

ENV LANG "en_US.UTF-8"
ENV MAKE "/usr/bin/make"
ENV CCACHE_WRAPPERSDIR "/usr/libexec/ccache-wrappers"
