FROM archlinux/base as base

WORKDIR /voronoi

COPY . /voronoi

RUN pacman -Sy --noconfirm - < archlinux.requirements

FROM base as pybase

RUN pip install --trusted-host pypi.python.org -r python.requirements

FROM pybase as dev

RUN mkdir build
RUN cmake -S . -B build
RUN make -C build
