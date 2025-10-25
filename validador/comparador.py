import re

def comparar_resultados(arquivo_c, arquivo_validador, saida_relatorio="validador/saidas/comparacao_resultados.txt"):
    def extrair_metricas(caminho):
        with open(caminho, 'r', encoding='utf-8') as f:
            conteudo = f.read()

        custo_match = re.search(r'Custo total:\s*(\d+)', conteudo)
        visitados_match = re.search(r'Nos visitados:\s*(\d+)', conteudo) # Para o arquivo validador
        tamanho_match = re.search(r'Tamanho do caminho:\s*(\d+)', conteudo)
        caminho_coords = re.findall(r'\((\d+),\s*(\d+)\)', conteudo)
        caminho_coords = [(int(x), int(y)) for x, y in caminho_coords]

        custo = int(custo_match.group(1)) if custo_match else None
        visitados = int(visitados_match.group(1)) if visitados_match else None
        tamanho = int(tamanho_match.group(1)) if tamanho_match else None

        return custo, visitados, tamanho, caminho_coords

    # Extrai métricas
    c_custo, c_visitados, c_tam, c_caminho = extrair_metricas(arquivo_c)
    v_custo, v_visitados, v_tam, v_caminho = extrair_metricas(arquivo_validador)

    # Monta relatório
    relatorio = []
    relatorio.append("📊 COMPARAÇÃO ENTRE RESULTADOS\n")

    relatorio.append(f"Custo total:      C={c_custo} | Validador={v_custo} | {'✅ Iguais' if (c_custo is not None and v_custo is not None and c_custo == v_custo) else '❌ Diferentes'}")
    relatorio.append(f"Nos visitados:    C={c_visitados} | Validador={v_visitados} | {'✅ Iguais' if (c_visitados is not None and v_visitados is not None and c_visitados == v_visitados) else '⚠️ Diferentes'}")
    relatorio.append(f"Tamanho caminho:  C={c_tam} | Validador={v_tam} | {'✅ Iguais' if (c_tam is not None and v_tam is not None and c_tam == v_tam) else '❌ Diferentes'}")

    # Comparação do caminho completo
    if c_caminho == v_caminho:
        relatorio.append("\nCaminho: ✅ Idêntico")
    else:
        relatorio.append("\nCaminho: ⚠️ Diferente")
        relatorio.append("\nDiferenças detalhadas:")
        # Find the minimum length to avoid index errors
        min_len = min(len(c_caminho), len(v_caminho))
        for i in range(min_len):
            if c_caminho[i] != v_caminho[i]:
                relatorio.append(f"  Passo {i+1}: C={c_caminho[i]} | Validador={v_caminho[i]}")

        # If one is longer than the other
        if len(c_caminho) != len(v_caminho):
            relatorio.append(f"\n  ⚠️ Comprimentos diferentes ({len(c_caminho)} vs {len(v_caminho)})")
            # Print the remaining steps
            if len(c_caminho) > min_len:
                relatorio.append(f"  Caminho C extra: {c_caminho[min_len:]}")
            else:
                relatorio.append(f"  Caminho Validador extra: {v_caminho[min_len:]}")


    relatorio.append("\n✅ Comparação concluída.\n")

    # Escreve o relatório em arquivo
    with open(saida_relatorio, 'w', encoding='utf-8') as f:
        f.write('\n'.join(relatorio))

    # Também imprime no terminal
    print("\n".join(relatorio))
    print(f"\n📁 Relatório salvo em: {saida_relatorio}")

# Exemplo de uso:
resultado_c = "codigo_c/saidas/resultado_c.txt"
resultado_validador = "validador/saidas/resultado_validador.txt"
comparar_resultados(resultado_c, resultado_validador)