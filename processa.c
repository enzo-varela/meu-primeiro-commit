#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_BUFFER 4
#define NUM_DADOS_POR_LINHA 3

double media_movel_ponderada(double buffer[], double pesos[], int tamanho_ativo) {
    double soma_valores = 0.0;
    double soma_pesos = 0.0;

    for (int i = 0; i < tamanho_ativo; i++) {
        soma_valores += buffer[i] * pesos[i];
        soma_pesos += pesos[i];
    }
    
    if (soma_pesos == 0.0) {
        return 0.0;
    }
    
    return soma_valores / soma_pesos;
}

int main() {
    double height_lida, pressure_lida, temperature_lida;

    FILE *arquivo_entrada = fopen("dados.txt", "r+"); 
    if (!arquivo_entrada) {
        perror("Erro ao abrir dados");
        return 1;
    }

    FILE *arquivo_saida = fopen("output.txt", "w");
    if (!arquivo_saida) {
        perror("Erro ao criar output.txt");
        fclose(arquivo_entrada);
        return 1;
    }

    double pesos[TAMANHO_BUFFER];
    for (int i = 0; i < TAMANHO_BUFFER; i++) {
        pesos[i] = (double)i + 1.0;
    }

    double buffer_height[TAMANHO_BUFFER] = {0.0};
    int indice_height = 0;
    int contagem_height = 0;

    double buffer_pressure[TAMANHO_BUFFER] = {0.0};
    int indice_pressure = 0;
    int contagem_pressure = 0;

    double buffer_temperature[TAMANHO_BUFFER] = {0.0};
    int indice_temperature = 0;
    int contagem_temperature = 0;
    
    fprintf(arquivo_saida, 
        "MEDIA_MOVEL_height, MEDIA_MOVEL_pressure, MEDIA_MOVEL_temperature\n"
    );

    while (fscanf(arquivo_entrada, "%lf %lf %lf", 
                  &height_lida, &pressure_lida, &temperature_lida) == NUM_DADOS_POR_LINHA) {
        
        double media_height, media_pressure, media_temperature;

        buffer_height[indice_height] = height_lida;
        indice_height = (indice_height + 1) % TAMANHO_BUFFER;
        if (contagem_height < TAMANHO_BUFFER) {
            contagem_height++;
        }
        media_height = media_movel_ponderada(buffer_height, pesos, contagem_height);

        buffer_pressure[indice_pressure] = pressure_lida;
        indice_pressure = (indice_pressure + 1) % TAMANHO_BUFFER;
        if (contagem_pressure < TAMANHO_BUFFER) {
            contagem_pressure++;
        }
        media_pressure = media_movel_ponderada(buffer_pressure, pesos, contagem_pressure);

        buffer_temperature[indice_temperature] = temperature_lida;
        indice_temperature = (indice_temperature + 1) % TAMANHO_BUFFER;
        if (contagem_temperature < TAMANHO_BUFFER) {
            contagem_temperature++;
        }
        media_temperature = media_movel_ponderada(buffer_temperature, pesos, contagem_temperature);

        fprintf(arquivo_saida, 
                "%.2lf, %.2lf, %.2lf\n", 
                media_height, media_pressure, media_temperature);
    }

    fclose(arquivo_entrada);
    fclose(arquivo_saida);

    printf("Dados do filtro estao no output.txt\n");
    return 0;
}