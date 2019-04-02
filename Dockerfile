FROM archlinux/base as base

WORKDIR /base

COPY archlinux.requirements /base/

RUN pacman -Sy --noconfirm - < archlinux.requirements

FROM base as pybase

COPY python.requirements /base/
RUN pip install --trusted-host pypi.python.org -r python.requirements

FROM pybase as dev

WORKDIR /voronoi
COPY . /voronoi
RUN mkdir build
RUN cmake -S . -B build
RUN make -C build
