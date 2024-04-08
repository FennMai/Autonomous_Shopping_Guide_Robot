

    // create the driver instance
	ILidarDriver * drv = *createLidarDriver();
    
    if (!drv) {
        fprintf(stderr, "insufficent memory, exit\n");
        exit(-2);
    }    