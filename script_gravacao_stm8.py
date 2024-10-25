import requests
import os
import subprocess

# Defina as variáveis do GitHub
REPO_OWNER = "Off-1357"
REPO_NAME = "JIGA_FAST_MANUAL"
FILE_PATH = "Debug/Exe/JIGA_MANUAL_PWM.hex"  # Caminho exato para o arquivo no repositório

# Caminho para o STVP_CmdLine.exe
STVP_CMD_PATH = "C:\Program Files (x86)\STMicroelectronics\st_toolset\stvp/STVP_CmdLine.exe"

# Diretório temporário simples para o arquivo .hex
TEMP_DIR = "C:/Temp"
if not os.path.exists(TEMP_DIR):
    os.makedirs(TEMP_DIR)
TEMP_HEX_PATH = os.path.join(TEMP_DIR, "JIGA_MANUAL_PWM.hex")

# Função para baixar o arquivo .hex do GitHub (sem autenticação)
def baixar_arquivo_hex():
    url = f"https://raw.githubusercontent.com/{REPO_OWNER}/{REPO_NAME}/main/{FILE_PATH}"
    response = requests.get(url)
    if response.status_code == 200:
        return response.content
    else:
        print("Erro ao baixar o arquivo:", response.status_code, response.text)
        return None

# Função para gravar o firmware no STM8
def gravar_no_microcontrolador(hex_file_path):
    comando = f'"{STVP_CMD_PATH}" -BoardName=ST-LINK -Port=USB -Device=STM8S003F3 -ProgMode=SWIM -FileProg="{hex_file_path}" -verif -no_log'
    subprocess.run(comando, shell=True)

def main():
    while True:
        conteudo_hex = baixar_arquivo_hex()
        if conteudo_hex is None:
            print("Não foi possível baixar o arquivo .hex.")
            break

        # Salva o conteúdo do arquivo no caminho temporário
        with open(TEMP_HEX_PATH, "wb") as temp_file:
            temp_file.write(conteudo_hex)

        print(f"Arquivo .hex salvo em: {TEMP_HEX_PATH}")

        # Gravar no microcontrolador
        gravar_no_microcontrolador(TEMP_HEX_PATH)

        # Aguardar Enter para repetir o processo
        input("Pressione Enter para repetir o processo ou Ctrl+C para sair...")

if __name__ == "__main__":
    main()
