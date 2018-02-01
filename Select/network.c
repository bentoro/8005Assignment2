void startClient(const char *ip, const char *portString, int inputFD) {
    network_init();

    int serverSock;
    if (ip == NULL) {
        char *address = getUserInput("Enter the server's address: ");
        serverSock = establishConnection(address, portString);
        free(address);
    } else {
        serverSock = establishConnection(ip, portString);
    }

    if (serverSock == -1) {
        fprintf(stderr, "Unable to connect to server\n");
        goto clientCleanup;
    }

    setNonBlocking(serverSock);

    size_t clientNum = addClient(serverSock);

    struct client *serverEntry = &clientList[clientNum];

    unsigned char *sharedSecret = exchangeKeys(&serverEntry->socket);

    debug_print_buffer("Shared secret: ", sharedSecret, SYMMETRIC_KEY_SIZE);

    int epollfd = createEpollFd();

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET | EPOLLEXCLUSIVE;
    ev.data.ptr = serverEntry;

    addEpollSocket(epollfd, serverSock, &ev);

    pthread_t readThread;
    pthread_create(&readThread, NULL, eventLoop, &epollfd);

    unsigned char input[4096];
    while(isRunning) {
        int n = read(inputFD, input, 4096);
        if (n > 0) {
            sendEncryptedUserData(input, n, serverEntry);
        } else {
            printf("Read returned zero or error\n");
            break;
        }
    }

clientCleanup:
    close(epollfd);
    close(inputFD);
    network_cleanup();
}

void startServer(const int inputFD) {
    int maxfd;
    fd_set read, master;
    network_init();
    if(epoll){
      int epollfd = createEpollFd();

      struct epoll_event ev;
      ev.events = EPOLLIN | EPOLLET | EPOLLEXCLUSIVE;
      ev.data.ptr = NULL;

      setNonBlocking(listenSock);

      addEpollSocket(epollfd, listenSock, &ev);

      pthread_t threads[7];
      for (size_t i = 0; i < 7; ++i) {
          pthread_create(&threads[i], NULL, eventLoop, &epollfd);
      }

      //TODO: Create threads here instead of calling eventloop directly
      eventLoop(&epollfd);

      close(epollfd);
      close(inputFD);
      network_cleanup();
    } else {
      FD_ZERO(&read);
      maxfd = listensoc;
      FD_SET(listensoc, &read);

      setNonBlocking(listenSock);
      // keep threads?
      pthread_t threads[7];
      for (size_t i = 0; i < 7; ++i) {
          pthread_create(&threads[i], NULL, eventLoop, &epollfd);
      }
      //change variable?
      eventLoop(&epollfd);

      close(epollfd);
      close(inputFD);
      network_cleanup();
    }

}

void *eventLoop(fd_set* fd, fd_set* backup, int* maxfd) {
    while(isRunning){
            memcpy(&backup, &fd, sizeof(read));
            //no timeout set
            n = select(maxfd+1,&backup, NULL, NULL, NULL);

            if(n < 0){
                perror("Select error");
                break;
            }
            //file descriptor is ready
            for(i = 0; i <= maxfd; i++){
                //check which file descriptor is ready
                if(FD_ISSET(i, &backup)){
                    if(i == (server->GetSocket())){
                    handleIncomingConnection(&master, &maxfd);
                    } else {
                    //incoming data on socket
                    handleIncomingPacket(i);
                    }
                }
            }
        }
}

void handleIncomingConnection(fd_set* fd, int* maxfd) {
    for(;;) {
        //listensock is the server listening socket?
        int sock = accept(listenSock, NULL, NULL);
        if (sock == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                //No incoming connections, ignore the error
                break;
            }
            fatal_error("accept");
        }
        setNonBlocking(sock);

        size_t newClientIndex = addClient(sock);

        unsigned char *secretKey = exchangeKeys(&clientList[newClientIndex].socket);
        debug_print_buffer("Shared secret: ", secretKey, HASH_SIZE);

        createSelectFd(fd, sock, maxfd);
    }
}
