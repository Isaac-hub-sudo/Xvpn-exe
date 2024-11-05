#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define VPN_DIR "C:/Users/IsaacDeOliveiraMaced/Downloads/Xvpn-master/Xvpn/vpndir"  // Corrigido com barras normais

// Função para imprimir texto centralizado com cor
void printCentered(const char *text, int width, const char *color) {
    int len = strlen(text);
    int padding = (width - len) / 2;
    printf("%s%*s%s%s\n", color, padding, text, "\033[0m", "");
}

int main() {
    // Definir a largura da tela (pode ser ajustado ou dinamicamente obtido)
    int COL = 80;  // Tamanho fixo, você pode usar um comando como `tput cols` para obter dinamicamente no C

    // Títulos iniciais, em cores
    printCentered("|                                                                                   |", COL, "\033[34m");
    printCentered("|                                                                                   |", COL, "\033[34m");
    printCentered(" ->Xvpn desenvolvido por Jean Koffi", COL, "\033[34m");

    // Exibir texto colorido
    char *echo[] = {
        " X     P      X           X       XVPNXVPN      X     X", 
        "  V   V        V         V        V       X     VV    V", 
        "   P P          P       P         PVPNXVPN      P P   P", 
        "    N            N     N          N             N  N  N", 
        "   X X            X   X           X             X   X X", 
        "  V   V            V V            V             V    VV", 
        " P     P            P             P             P     P"
    };

    for (int i = 0; i < 7; i++) {
        if (i % 2 == 0) {
            printCentered(echo[i], COL, "\033[33m");
        } else {
            printCentered(echo[i], COL, "\033[97m");
        }
    }

    // Abrir o diretório e ler os arquivos
    DIR *dir = opendir(VPN_DIR);
    if (!dir) {
        perror("Erro ao abrir o diretório");
        return 1;
    }

    struct dirent *entry;
    int index = 0;
    char *vpn_files[100];  // Array para armazenar os arquivos .ovpn encontrados
    printf("\nEscolha sua lista de VPN:\n");

    // Ler os arquivos do diretório
    while ((entry = readdir(dir)) != NULL) {
        // Verificar se o arquivo tem a extensão .ovpn
        if (strstr(entry->d_name, ".ovpn") != NULL) {
            vpn_files[index] = entry->d_name;
            printf("%d - %s\n", index + 1, entry->d_name);
            index++;
        }
    }

    closedir(dir);  // Fechar o diretório após leitura

    // Se não encontrar arquivos .ovpn
    if (index == 0) {
        printf("Nenhum arquivo .ovpn encontrado no diretório.\n");
        return 1;
    }

    // Obter a escolha do usuário
    int choice;
    printf("Escolha o número do VPN que deseja usar: ");
    
    // Garantir que a escolha seja válida
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > index) {
        printf("Escolha inválida! Por favor, digite um número entre 1 e %d: ", index);
        while(getchar() != '\n');  // Limpar o buffer de entrada
    }

    // Construir o caminho completo do arquivo de configuração
    char conf_file[256];
    snprintf(conf_file, sizeof(conf_file), "%s/%s", VPN_DIR, vpn_files[choice - 1]);

    // Verificar se o arquivo de configuração existe
    struct stat statbuf;
    if (stat(conf_file, &statbuf) == 0) {
        printf("Arquivo de configuração encontrado: %s\n", conf_file);
        
        // Comando para executar o OpenVPN
        char cmd[512];
        snprintf(cmd, sizeof(cmd), "openvpn --config \"%s\"", conf_file);
        
        // Executar o comando openvpn
        int result = system(cmd);
        if (result != 0) {
            printf("Erro ao tentar conectar com o OpenVPN. Verifique o comando e a configuração.\n");
        }
    } else {
        printf("Arquivo de configuração não encontrado: %s\n", conf_file);
    }

    return 0;
}
