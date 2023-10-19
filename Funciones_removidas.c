//_________________________________________________________________________________
// Prototypes (from remota_globals.h)
//_________________________________________________________________________________

esp_err_t tablePrint(uint16_t *table, uint8_t size);
esp_err_t tablePrintFloat(float *table, uint8_t size);
esp_err_t tablesUnload(varTables_t *tables);
esp_err_t readAnalogTable(varTables_t *Tables, uint8_t tbl);
esp_err_t readDigitalTable(varTables_t *Tables, uint8_t tbl);
esp_err_t readConfigTable(varTables_t *Tables, uint8_t tbl);
esp_err_t readAuxTable(varTables_t *Tables, uint8_t tbl);
esp_err_t readAllTables(varTables_t *Tables);
esp_err_t readAnalogData(varTables_t *Tables, uint8_t tbl, uint8_t dataIndex);
esp_err_t readDigitalData(varTables_t *Tables, uint8_t tbl, uint8_t dataIndex);
esp_err_t readConfigData(varTables_t *Tables, uint8_t tbl, uint8_t dataIndex);
esp_err_t readAuxData(varTables_t *Tables, uint8_t tbl, uint8_t dataIndex);
esp_err_t writeAnalogTable(varTables_t *Tables, uint8_t tbl);
esp_err_t writeDigitalTable(varTables_t *Tables, uint8_t tbl);
esp_err_t writeConfigTable(varTables_t *Tables, uint8_t tbl);
esp_err_t writeAuxTable(varTables_t *Tables, uint8_t tbl);
esp_err_t writeAnalogData(uint8_t tbl, uint8_t dataIndex, uint16_t payload);
esp_err_t writeDigitalData(uint8_t tbl, uint8_t dataIndex, uint16_t payload);
esp_err_t writeConfigData(uint8_t tbl, uint8_t dataIndex, uint16_t payload);
esp_err_t writeAuxData(uint8_t tbl, uint8_t dataIndex, uint16_t payload);




//_________________________________________________________________________________
// Definitions (from main.c)
//_________________________________________________________________________________


esp_err_t tablePrint(uint16_t *table, uint8_t size){
	//static const char TAG[] = "Table print";
    esp_log_level_set(TAG, CFG_REMOTA_LOG_LEVEL);
    char text[350] = {'\0'};
    char buffer[10] = {'\0'};
    for (int i=0; i < size; i++){
        sprintf(buffer, "%u ",table[i]);
        strcat(text, buffer);
        strcat(text, " ");
    }
    ESP_LOGI(TAG, "%s\n", text);
    return ESP_OK;
} 

esp_err_t tablePrintFloat(float *table, uint8_t size){
	//static const char TAG[] = "Table print";
    esp_log_level_set(TAG, CFG_REMOTA_LOG_LEVEL);
    char text[350] = {'\0'};
    char buffer[10] = {'\0'};
    for (int i=0; i < size; i++){
        sprintf(buffer, "%0.2f ",table[i]);
        strcat(text, buffer);
        strcat(text, " ");
    }
    ESP_LOGI(TAG, "%s\n", text);
    return ESP_OK;
} 

esp_err_t tablesUnload(varTables_t *tables){
    //static const char TAG[] = "tablesUnload";
    for (int i=0; i<tables->numAnTbls; i++){  //Libera primero cada vector (int*)
		free(tables->anTbl[i]);
	}
	free(tables->anTbl);                  //Libera el vector de apuntadores (int**)

    for (int i=0; i<tables->numDigTbls; i++){  //Libera primero cada vector (int*)
		free(tables->digTbl[i]);
	}
	free(tables->digTbl);                  //Libera el vector de apuntadores (int**)
    
    for (int i=0; i<tables->numConfigTbls; i++){  //Libera primero cada vector (int*)
		free(tables->configTbl[i]);
	}
	free(tables->configTbl);                  //Libera el vector de apuntadores (int**)

    for (int i=0; i<tables->numAuxTbls; i++){  //Libera primero cada vector (int*)
		free(tables->auxTbl[i]);
	}
	free(tables->auxTbl);                  //Libera el vector de apuntadores (int**)

    ESP_LOGI(TAG, "Memoria liberada");
    return ESP_OK;
} 

esp_err_t readAnalogTable(varTables_t *Tables, uint8_t tbl){
    spi_transaction_counter();
    
    sendbuf[0] = 1;
    sendbuf[1] = tbl;
    sendbuf[2] = 0;
    sendbuf[3] = 0;
            
    spi_write(sendbuf, 4);
    
    esp_err_t res = spi_receive(Tables->anSize);
    if (res != ESP_OK){             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    if (recvbuf[0] != 0xFFFF){      //Command not recognized error condition check
        for (int j=0; j < Tables->anSize; j++){
            Tables->anTbl[tbl][j] = recvbuf[j];
            recvbuf[j]=0;
        }
    }
    else {
        printf("Communication error! try again...\n");
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }
    
    return ESP_OK;
}

esp_err_t readDigitalTable(varTables_t *Tables, uint8_t tbl){
    spi_transaction_counter();
    sendbuf[0] = 2;
    sendbuf[1] = tbl;
    sendbuf[2] = 0;
    sendbuf[3] = 0;
        
    spi_write(sendbuf, 4);   
    
    esp_err_t res = spi_receive(Tables->digSize);
    if (res != ESP_OK){             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    if (recvbuf[0] != 0xFFFF){      //Command not recognized error condition check
        for (int j=0; j < Tables->digSize; j++){
            Tables->digTbl[tbl][j] = recvbuf[j];
            recvbuf[j]=0;
        }
    }
    else {
        printf("Communication error! try again...\n");
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t readConfigTable(varTables_t *Tables, uint8_t tbl){
    spi_transaction_counter();
    sendbuf[0] = 9;
    sendbuf[1] = tbl;
    sendbuf[2] = 0;
    sendbuf[3] = 0;
        
    spi_write(sendbuf, 4);  

    esp_err_t res = spi_receive(Tables->configSize);
    if (res != ESP_OK){             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    if (recvbuf[0] != 0xFFFF){      //Command not recognized error condition check
        for (int j=0; j < Tables->configSize; j++){
            Tables->configTbl[tbl][j] = recvbuf[j];
            recvbuf[j]=0;
        }
    }
    else {
        printf("Communication error! try again...\n");
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t readAuxTable(varTables_t *Tables, uint8_t tbl){
    spi_transaction_counter();
    sendbuf[0] = 10;
    sendbuf[1] = tbl;
    sendbuf[2] = 0;
    sendbuf[3] = 0;
        
    spi_write(sendbuf, 4);

    esp_err_t res = spi_receive(Tables->auxSize);
    if (res != ESP_OK){             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    if (recvbuf[0] != 0xFFFF){      //Command not recognized error condition check
        for (int j=0; j < Tables->auxSize; j++){
            Tables->auxTbl[tbl][j] = recvbuf[j];
            recvbuf[j]=0;
        }
    }
    else {
        printf("Communication error! try again...\n");
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t readAllTables(varTables_t *Tables){
    for (int i=0; i < Tables->numAnTbls; i++){
        esp_err_t r = readAnalogTable(Tables, i);
        if (r == ESP_OK)
            tablePrint(Tables->anTbl[i], Tables->anSize);
    }

    for (int i=0; i < Tables->numDigTbls; i++){
        esp_err_t r = readDigitalTable(Tables, i);
        if (r == ESP_OK)
            tablePrint(Tables->digTbl[i], Tables->digSize);
    }
    return ESP_OK;
}

esp_err_t readAnalogData(varTables_t *Tables, uint8_t tbl, uint8_t dataIndex){
    spi_transaction_counter();
    sendbuf[0] = 3;
    sendbuf[1] = tbl;
    sendbuf[2] = dataIndex;
    sendbuf[3] = 0;
        
    spi_write(sendbuf, 4);

    esp_err_t res = spi_receive(1);
    if (res != ESP_OK){             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }
    
    if (recvbuf[0] != 0xFFFF){      //Command not recognized error condition check
        Tables->anTbl[tbl][dataIndex] = recvbuf[0];
        recvbuf[0]=0;
        printf("%u\n", (uint16_t)Tables->anTbl[tbl][dataIndex]);
    }
    else {
        printf("Communication error! try again...\n");
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK;

}

esp_err_t readDigitalData(varTables_t *Tables, uint8_t tbl, uint8_t dataIndex){
    spi_transaction_counter();
    sendbuf[0] = 4;
    sendbuf[1] = tbl;
    sendbuf[2] = dataIndex;
    sendbuf[3] = 0;
        
    spi_write(sendbuf, 4);

    esp_err_t res = spi_receive(1);
    if (res != ESP_OK){             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    if (recvbuf[0] != 0xFFFF){      //Command not recognized error condition check
        Tables->digTbl[tbl][dataIndex] = recvbuf[0];
        recvbuf[0]=0;
        printf("%u\n", (uint16_t)Tables->digTbl[tbl][dataIndex]);
    }
    else {
        printf("Communication error! try again...\n");
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t readConfigData(varTables_t *Tables, uint8_t tbl, uint8_t dataIndex){
    spi_transaction_counter();
    sendbuf[0] = 15;
    sendbuf[1] = tbl;
    sendbuf[2] = dataIndex;
    sendbuf[3] = 0;
        
    spi_write(sendbuf, 4);
    
    esp_err_t res = spi_receive(1);
    if (res != ESP_OK){             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    if (recvbuf[0] != 0xFFFF){      //Command not recognized error condition check
        Tables->configTbl[tbl][dataIndex] = recvbuf[0];
        recvbuf[0]=0;
        printf("%u\n", (uint16_t)Tables->configTbl[tbl][dataIndex]);
    }
    else {
        printf("Communication error! try again...\n");
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t readAuxData(varTables_t *Tables, uint8_t tbl, uint8_t dataIndex){
    spi_transaction_counter();
    sendbuf[0] = 16;
    sendbuf[1] = tbl;
    sendbuf[2] = dataIndex;
    sendbuf[3] = 0;
        
    spi_write(sendbuf, 4);
    
    esp_err_t res = spi_receive(1);
    if (res != ESP_OK){             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    if (recvbuf[0] != 0xFFFF){      //Command not recognized error condition check
        Tables->auxTbl[tbl][dataIndex] = recvbuf[0];
        recvbuf[0]=0;
        printf("%u\n", (uint16_t)Tables->auxTbl[tbl][dataIndex]);
    }
    else {
        printf("Communication error! try again...\n");
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t writeAnalogTable(varTables_t *Tables, uint8_t tbl){
    spi_transaction_counter();
    sendbuf[0] = 5;
    sendbuf[1] = tbl;
    sendbuf[2] = 0;
    sendbuf[3] = 0;
        
    spi_write(sendbuf, 4);

    for (int i=0; i<s3Tables.anSize; i++)
        sendbuf[i] = s3Tables.anTbl[tbl][i];
    spi_write(sendbuf, s3Tables.anSize);

    esp_err_t res = spi_receive(1);
    if ((res != ESP_OK) || (recvbuf[0] == 0xFFFF)) {             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }
    
    return ESP_OK;
}

esp_err_t writeDigitalTable(varTables_t *Tables, uint8_t tbl){
    spi_transaction_counter();
    sendbuf[0] = 6;
    sendbuf[1] = tbl;
    sendbuf[2] = 0;
    sendbuf[3] = 0;
        
    spi_write(sendbuf, 4);

    for (int i=0; i<s3Tables.digSize; i++)
        sendbuf[i] = s3Tables.digTbl[tbl][i];
    spi_write(sendbuf, s3Tables.digSize);

    esp_err_t res = spi_receive(1);
    if ((res != ESP_OK) || (recvbuf[0] == 0xFFFF)) {             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK; 
}

esp_err_t writeConfigTable(varTables_t *Tables, uint8_t tbl){
    spi_transaction_counter();
    sendbuf[0] = 11;
    sendbuf[1] = tbl;
    sendbuf[2] = 0;
    sendbuf[3] = 0;
        
    spi_write(sendbuf, 4);

    for (int i=0; i<s3Tables.configSize; i++)
        sendbuf[i] = s3Tables.configTbl[tbl][i];
    spi_write(sendbuf, s3Tables.configSize);

    esp_err_t res = spi_receive(1);
    if ((res != ESP_OK) || (recvbuf[0] == 0xFFFF)) {             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t writeAuxTable(varTables_t *Tables, uint8_t tbl){
    spi_transaction_counter();
    sendbuf[0] = 12;
    sendbuf[1] = tbl;
    sendbuf[2] = 0;
    sendbuf[3] = 0;
        
    spi_write(sendbuf, 4);

    for (int i=0; i<s3Tables.auxSize; i++)
        sendbuf[i] = s3Tables.auxTbl[tbl][i];
    spi_write(sendbuf, s3Tables.auxSize);

    esp_err_t res = spi_receive(1);
    if ((res != ESP_OK) || (recvbuf[0] == 0xFFFF)) {             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t writeAnalogData(uint8_t tbl, uint8_t dataIndex, uint16_t payload){
    spi_transaction_counter();
    sendbuf[0] = 7;
    sendbuf[1] = tbl;
    sendbuf[2] = dataIndex;
    sendbuf[3] = payload;
       
    spi_write(sendbuf, 4);

    esp_err_t res = spi_receive(1);
    if ((res != ESP_OK) || (recvbuf[0] == 0xFFFF)) {             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t writeDigitalData(uint8_t tbl, uint8_t dataIndex, uint16_t payload){
    spi_transaction_counter();
    sendbuf[0] = 8;
    sendbuf[1] = tbl;
    sendbuf[2] = dataIndex;
    sendbuf[3] = payload;
        
    spi_write(sendbuf, 4);

    esp_err_t res = spi_receive(1);
    if ((res != ESP_OK) || (recvbuf[0] == 0xFFFF)) {             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t writeConfigData(uint8_t tbl, uint8_t dataIndex, uint16_t payload){
    spi_transaction_counter();
    sendbuf[0] = 13;
    sendbuf[1] = tbl;
    sendbuf[2] = dataIndex;
    sendbuf[3] = payload;
        
    spi_write(sendbuf, 4);

    esp_err_t res = spi_receive(1);
    if ((res != ESP_OK) || (recvbuf[0] == 0xFFFF)) {             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t writeAuxData(uint8_t tbl, uint8_t dataIndex, uint16_t payload){
    spi_transaction_counter();
    sendbuf[0] = 14;
    sendbuf[1] = tbl;
    sendbuf[2] = dataIndex;
    sendbuf[3] = payload;
        
    spi_write(sendbuf, 4);

    esp_err_t res = spi_receive(1);
    if ((res != ESP_OK) || (recvbuf[0] == 0xFFFF)) {             //Checksum error condition check
        SPI_ERROR_COUNT++;
        return ESP_FAIL;
    }

    return ESP_OK;
}