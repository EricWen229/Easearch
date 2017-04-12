FROM ericwen229/cppcms

MAINTAINER Eric Wen "eericwen@icloud.com"

ENV REFRESHED_AT 20170412

COPY . /root/easearch/
WORKDIR /root/easearch
RUN make

ENV LD_LIBRARY_PATH=/usr/local/lib

EXPOSE 8080
ENTRYPOINT ["make", "run"]

