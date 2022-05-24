#include <stdint.h>
#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#define precoLux 500
#define precoInt 250
#define precoBas 150

typedef struct {
    int cnh;
    char nome[30];
    int fidelidade;
    char placa[30];
}Cliente;

typedef struct{
    char placa[30];
    int categoria;
    char modelo[30];
    char marca[30];
    int ano;
    int kmCarro;
    int disponibilidade;
}Carro;

//prototipos 

int PainelInicial();
int FazerLoc(Cliente*clientes, Carro*carros);
int EncerraLoc(Cliente*clientes, Carro*carros);
int buscaCliente(Cliente*clientes, int cnh);
int buscaCarro(Carro*carros, char *placaCarro);
int buscaVeiculoDisponivel(Carro*frota, int quantidadeCarro, int categoria);
int verificaString( char str[10],  char str2[10]);
int ArqCarros(Carro*frota);
int ArqClientes(Cliente*clientes);
int diaria(int dias, int categoria, int km );
int relatorio(Carro *frota,int quantidadeCarro);
void GravaCarros(Carro *frota, int quantidadeCarro);
void GravaClientes(Cliente *clientes, int quantidadedeClientes);


int PainelInicial(){
    Cliente*clientes;
    Carro*carros;
    carros=malloc(sizeof(Carro)*100);
    clientes=malloc(sizeof(Cliente)*100);
    ArqCarros(carros);
    ArqClientes(clientes);
    printf("\n");
    int op;
    
    do{
        printf("Bem vindo a locadora de carros da Unipampa.\n");
        printf("Selecione uma das opcoes abaixo para prosseguir.\n");
        printf("Opcao 1: Realizar uma locacao de veiculo. \n");
        printf("Opcao 2: Finalizar uma locacao de veiculo. \n");
        printf("Opcao 3: Relatorio\n");
        printf("Opcao 4: Fechar o programa.\n");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            FazerLoc(clientes, carros);
            break;
        case 2:
            EncerraLoc(clientes, carros);
            break;
        case 3:
            relatorio(carros,5);
            break;
        case 4: 
            exit (1);
            break;
        default:
            printf("Escolha uma das opcoes\n");
            break;
        }
    } while (op != 4);
    free(carros);
    free(clientes);
 return 0;   
}

int FazerLoc(Cliente*clientes, Carro*carros){
    int cnh;
    char placa[7];
    int categoria;
    int dia, mes,ano;
    printf("Informe a CNH do cliente:\n");
    scanf("%d", &cnh);
    int vetorCliente =buscaCliente(clientes, cnh);
    if (vetorCliente == -1){
        return -1;
    }
    
    printf("Pontos disponivels: %d \n\n", clientes[vetorCliente].fidelidade);
   
    if(clientes[vetorCliente].fidelidade<=300){
      categoria =1;
    }
    if(clientes[vetorCliente].fidelidade>=300 && clientes[vetorCliente].fidelidade<=700){
      categoria =2;
    }
    if(clientes[vetorCliente].fidelidade>700){
      categoria =3;
    }
    buscaVeiculoDisponivel(carros,5,categoria);
    printf("Informe a placa do veiculo escolhido:\n");
    scanf("%s", placa);
    int vetorCarros = buscaCarro(carros, placa);
    if (vetorCarros == -1){
        printf("veiculo nao encontrado.\n");
        return -1;
    }
    carros[vetorCarros].disponibilidade = 0;
    strcpy(clientes[vetorCliente].placa, carros[vetorCarros].placa);
    printf ("Digite a data de locacao (Ex: dd/mm/aaaa): ");
    scanf("%d/%d/%d",&dia,&mes,&ano);
    printf("----------------------RELATORIO DE LOCACAO--------------------------------");
    printf("\nVeiculo locado para: %s. \nCNH: %d.\nPLACA do veiculo:%s.\nCategoria do veiculo (1 = basica, 2= intermediario, 3 luxo): %d  \nQuilometragem: %d.\nData de Locacao %d/%d/%d\n ",clientes[vetorCliente].nome, clientes[vetorCliente].cnh,carros[vetorCarros].placa,carros[vetorCarros].categoria,carros[vetorCarros].kmCarro,dia,mes,ano);
    printf("-------------------------FIM RELATORIO--------------------------");
    GravaCarros(carros, 5);
    GravaClientes(clientes, 5);
   return PainelInicial();

}

int EncerraLoc(Cliente*clientes, Carro*carros){
    int cnh;
    int categoria;
    do{
    printf("Informe a sua CNH: \n");
    scanf("%d", &cnh);
    int posCliente = buscaCliente(clientes, cnh);
    int posicaoCarro = buscaCarro(carros, clientes[posCliente].placa);
    if (posicaoCarro == -1)
    {
        printf("O cliente nao possui veiculos locados\n");
        return PainelInicial();
    }else{
    int dias;
    printf("Quantos dias o cliente ficou com o veículo?\n");
    scanf("%d", &dias);
    int kmAtual;
    printf("Qual o total de kilometros pelo cliente?\n");
    scanf("%d", &kmAtual);
     if (posCliente == -1){
        return -1;
    }
   
    if(clientes[posCliente].fidelidade<=300){
      categoria =1;
    }
    if(clientes[posCliente].fidelidade>=300 && clientes[posCliente].fidelidade<=700){
      categoria =2;
    }
    if(clientes[posCliente].fidelidade>700){
      categoria =3;
    }
    diaria(dias,categoria,kmAtual);
    carros[posicaoCarro].disponibilidade = 1;
    strcpy(clientes[posCliente].placa, "NULL");
  
    clientes[posCliente].fidelidade += kmAtual;
    carros[posicaoCarro].kmCarro += kmAtual;  
    GravaCarros(carros, 5);
    GravaClientes(clientes, 5);
    printf("Locacao encerrada.\n");
    return PainelInicial();
    }
    }while(cnh!=0);
}

int diaria(int dias, int categoria, int km ){
    int valordiaria;
    switch (categoria)
        {
        case 1 : //categoria basica valor do km excedido é 5 reais
        printf("categoria do cliente eh basica\n");
        valordiaria =precoBas;
        if (km>500){       
            valordiaria = (((km-500)*5)+ valordiaria); 
            printf("O valor a se pago eh: %d reais\n",valordiaria);
        }else{
        valordiaria =precoBas*dias;
        printf("O valor a se pago eh: %d reais\n",valordiaria);}
        break;

        case 2 ://categoria intermediar valor do km excedido é 2 reais
        printf("categoria do cliente eh intermediaria\n");
        valordiaria =precoInt;
        if (km>700){
            valordiaria = (((km-700)*2)+ valordiaria); 
            printf("O valor a se pago eh: %d reais\n",valordiaria);
        }else{
        valordiaria =precoInt*dias;
        printf("O valor a se pago eh: %d reais\n",valordiaria);}
        break;
        case 3 ://categoria lux valor do km excedido é 1 reais
         printf("categoria do cliente eh Luxo\n");
         valordiaria =precoLux;
         if (km>900){
            valordiaria= (km-900)*1 + valordiaria; 
             printf("O valor a se pago eh: %d reais\n",valordiaria);
        }else{
        valordiaria =precoLux*dias;
        printf("O valor a se pago eh: %d reais\n",valordiaria);}
        break;     
        }
        
}

int buscaCliente(Cliente*clientes, int cnh){
    int tamanho = 5;
    int retorna;
     for (int i = 0; i <tamanho; i++){
        if (clientes[i].cnh == cnh){
            retorna = i;
            return retorna;
        }         
    }
printf("CNH nao encontrada.\n");
return -1;
}

int buscaCarro(Carro*carros, char *placaCarro){
    int tamanho = 5;
    int retorna;
    for (int i = 0; i < tamanho; i++){
   verificaString(carros[i].placa,placaCarro); 
    if (!strcmp(carros[i].placa,placaCarro)){
            retorna =i;
            return retorna;
        }    
    }
   return -1;
}

int relatorio(Carro *frota,int quantidadeCarro){
    int i;
    fflush(stdin);
        printf ("\nO Relatorio de carros eh\n");
        printf("\nOs carros diponiveis sao:\n");
        for(int i =0; i< quantidadeCarro; i++){
        if(frota[i].disponibilidade == 1 ){
            printf("%s,", frota[i].placa);
            printf("%d,", frota[i].categoria);
            printf("%s, ", frota[i].modelo);
            printf("%s, ", frota[i].marca);
            printf("%d, ", frota[i].ano);
            printf("%d, ", frota[i].kmCarro);
            printf("%d.\n", frota[i].disponibilidade);
            puts(" ");
            }
        }
    
    printf("Veiculos indisponiveis.\n");
    for(int i =0; i< quantidadeCarro; i++){
        if(frota[i].disponibilidade == 0){
            printf("%s, ", frota[i].placa);
            printf("%d, ", frota[i].categoria);
            printf("%s, ", frota[i].modelo);
            printf("%s, ", frota[i].marca);
            printf("%d, ", frota[i].ano);
            printf("%d, ", frota[i].kmCarro);
            printf("%d.\n", frota[i].disponibilidade);
            puts(" ");
        }
    }
    
    return 1;
}

int buscaVeiculoDisponivel(Carro*frota, int quantidadeCarro, int categoria){
    int i=0;
    int preco;
    int cont =0;
  switch (categoria)
        {
        case 1 :
        preco =precoBas;
        printf("A categoria do cliente eh a basica.\n");
        printf("O valor da diaria eh %d reais.\n", preco);
        printf("Os carros diponiveis para essa categoria sao:\n\n");
        
        for(int i =0; i< quantidadeCarro; i++){
            if(frota[i].disponibilidade == 1 && frota[i].categoria ==1){
                printf("%s,", frota[i].placa);
                printf("%d,", frota[i].categoria);
                printf("%s, ", frota[i].modelo);
                printf("%s, ", frota[i].marca);
                printf("%d, ", frota[i].ano);
                printf("%d, ", frota[i].kmCarro);
                printf("%d.\n", frota[i].disponibilidade);
                puts(" ");
                cont++; 
                }
             }   
            if(cont ==0 ){
                       printf ("Nao existem carros disponiveis\n");
                       return PainelInicial();
                   }     
            break;
        case 2:
        preco =precoInt;
        cont = 0;
        printf("A categoria do cliente eh a intermediaria.\n");
        printf("O valor da diaria eh %d reais.\n", preco);
        printf("Os carros diponiveis para essa categoria sao:\n");
        for(int i =0; i< quantidadeCarro; i++){
            if(frota[i].disponibilidade == 1 && frota[i].categoria == 2 ){
                printf("%s,", frota[i].placa);
                printf("%d,", frota[i].categoria);
                printf("%s, ", frota[i].modelo);
                printf("%s, ", frota[i].marca);
                printf("%d, ", frota[i].ano);
                printf("%d, ", frota[i].kmCarro);
                printf("%d.\n", frota[i].disponibilidade);
                puts(" ");
                 cont++;
                }
            }
                if(cont ==0 ){
                       printf ("Nao existem carros disponiveis\n");
                       return PainelInicial();
                   }  
            break;
        case 3:
        preco =precoLux;
        cont = 0;
        printf("A categoria do cliente eh Luxo.\n");
        printf("O valor da diaria eh %d reais.\n", preco);
        printf("Os carros diponiveis para essa categoria sao:\n");
        for(int i =0; i< quantidadeCarro; i++){
            if(frota[i].disponibilidade == 1 && frota[i].categoria == 3 ){
                printf("%s,", frota[i].placa);
                printf("%d,", frota[i].categoria);
                printf("%s, ", frota[i].modelo);
                printf("%s, ", frota[i].marca);
                printf("%d, ", frota[i].ano);
                printf("%d, ", frota[i].kmCarro);
                printf("%d.\n", frota[i].disponibilidade);
                puts(" ");
                cont++;
                }      
        }
        if(cont ==0 ){
                       printf ("Nao existem carros disponiveis\n");
                       return PainelInicial();
                   }
        break;
    }
   return i;
}

int verificaString( char str[10],  char str2[10]){
    fflush(stdin);
    int flag =0;
    
    for(int i=0; str[i] != '\0' || str2[i] != '\0';i++){
        //Codigo destinado a remover o \n das strings vindas dos arquivos.
        if(str[i] == '\n'){
            memmove(str, str+1, strlen(str));
        }
        if(str[i] != str2[i]){
            flag = 1;
            return 1;
        }
       
    }
    if(flag = 0) return (0);
}

int ArqCarros(Carro*frota){
FILE *arq;
int i = 0;

arq = fopen("FrotaCarros.txt", "r");

int numerodeLinha =0;
int r;

if (arq == NULL){
printf("Problemas na Criação do arquivo\n");
    return 1;
}
fscanf(arq, "%d", &numerodeLinha);
//printf("%d", numerodeLinha);
do{
 r= fscanf(arq, "%29[^,],%d,%29[^,],%29[^,],%d,%d,%d,",frota[i].placa, &frota[i].categoria, frota[i].modelo, frota[i].marca, &frota[i].ano, &frota[i].kmCarro, &frota[i].disponibilidade); 
//printf("%s,%d,%s,%s,%d,%d,%d\n", frota[i].placa, frota[i].categoria, frota[i].modelo, frota[i].marca, frota[i].ano, frota[i].kmCarro, frota[i].disponibilidade);

if (r!=7 && !feof(arq)){
    printf("Formato invalido");
    return 1;
}

if (ferror(arq)){
    printf("Erro no arquivo");
    return 1;
}

i++;

} while (!feof(arq));

    fclose(arq);
}

int ArqClientes(Cliente*clientes){

FILE *arq;
int i = 0;

arq = fopen("Clientes.txt", "r");

int numerodeLinha;
int r;

if (arq == NULL){
printf("Problemas na Criação do arquivo\n");
    return 1;
}
fscanf(arq, "%d", &numerodeLinha);
do{
 r= fscanf(arq, "%d,%29[^,],%d,%29[^,],",&clientes[i].cnh, clientes[i].nome, &clientes[i].fidelidade, clientes[i].placa);
if (r!=4 && !feof(arq)){
    printf("Formato invalido");
    return 1;
}

if (ferror(arq)){
    printf("Erro no arquivo");
    return 1;
}

i++;

} while (!feof(arq));

    fclose(arq);
}

void GravaCarros(Carro *frota, int quantidadeCarro)
{   
    fflush(stdin);
    FILE *gravCar;
    int i= 0;
    
    int quantidade = 5;
    gravCar = fopen("FrotaCarros.txt", "w");

    if (gravCar == NULL)
    {
        printf("Problemas na Criação do arquivo\n");
        return 1;
    }
    fflush(stdin);
    fprintf(gravCar, "%d\n", quantidadeCarro);
    do
    {                                           //tirar o barra nova linha. para nao dar espacamento no arquivo.
        fprintf(gravCar, "%s,%d,%s,%s,%d,%d,%d,", frota[i].placa, frota[i].categoria, frota[i].modelo, frota[i].marca, frota[i].ano, frota[i].kmCarro, frota[i].disponibilidade);
        i++;
        quantidade--;
    } while (quantidade != 0);

    fclose(gravCar);
}

void GravaClientes(Cliente *clientes, int quantidadedeClientes)
{   
    fflush(stdin);
    FILE *gravCli;
    int i= 0;
    
    int quantidade = 5;
    gravCli = fopen("Clientes.txt", "w");

    if (gravCli == NULL)
    {
        printf("Problemas na Criação do arquivo\n");
        return 1;
    }
    fflush(stdin);
    fprintf(gravCli, "%d\n", quantidadedeClientes);
    do{
        fprintf(gravCli, "%d,%s,%d,%s,\n", clientes[i].cnh, clientes[i].nome, clientes[i].fidelidade, clientes[i].placa);
        i++;
        quantidade--;
    
    } while (quantidade != 0);

    fclose(gravCli);
}

int main(){
   
PainelInicial();
   
}



