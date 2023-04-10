FROM rockylinux:9.1

RUN dnf install -y \
    bash-completion \
    boost-devel \
    gcc-c++ \
    git \
    python3-devel \
    unzip \
    vim \
    && \
    dnf clean all

# pip のオートコンプリート
RUN pip completion --bash > /etc/profile.d/pip.sh

RUN { \
        echo "alias l.='ls -d .* --color=auto'"; \
        echo "alias ll='ls -l --color=auto'"; \
        echo "alias ls='ls --color=auto'"; \
    } > /etc/profile.d/my.sh

ENV PYTHON_VERSION=3.9