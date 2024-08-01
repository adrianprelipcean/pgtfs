# SPDX-FileCopyrightText: 2024 Adrian C. Prelipcean <adrianprelipceanc@gmail.com>
#
# SPDX-License-Identifier: CC-BY-NC-SA-4.0

FROM postgres:16.2
MAINTAINER Adrian C. Prelipcean

RUN apt-get update && \
  apt-get install -y --no-install-recommends \
    build-essential curl ca-certificates git gnupg \
    postgresql-server-dev-all \ 
    python3.10 python3-pip \
    doxygen gcc graphviz

COPY . ./

RUN make install

RUN pip install --break-system-packages -r requirements.txt 

# Open port 5432 so linked containers can see them
EXPOSE 5432

ENV DATABASE="" \
    HOST=db \
    PORT=5432 \
    USER="postgres" \
    PASSWORD="" \