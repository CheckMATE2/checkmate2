FROM lukasheinrich/cmbasefedora
ADD . /code/CheckMATE
RUN yum install -y autoconf automake libtool python2-numpy python2-scipy
RUN pip install -U scipy==0.11.0
RUN cd /code/CheckMATE && \
    autoreconf && \ 
    ./configure \
    --with-madgraph=/code/MG5_aMC_v2_6_3_2/  \
    --with-delphes=/code/delphes  \
    --with-rootsys=/usr/local  \
    --with-hepmc=/usr/local  \
    --with-pythia=/usr/local && \
    make -j4 && \
    make install


ENV MANPATH /usr/local/man
ENV LD_LIBRARY_PATH=/code/delphes:/usr/local/lib
ENV LIBPATH /usr/local/lib
ENV PATH /code/delphes:/usr/local/bin:/usr/local/sbin:/usr/sbin:/usr/bin:/sbin:/bin
ENV PYTHONPATH /code/delphes/python:/usr/local/lib
ENV SHLIB_PATH /usr/local/lib
