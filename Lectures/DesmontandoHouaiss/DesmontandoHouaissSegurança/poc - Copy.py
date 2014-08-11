def Desofuscar(giveMeBytes):
    # transformedBytes = Something(giveMeBytes)
    transformedBytes = [c + 0xb for c in giveMeBytes]
    bytesToString = [unichr(c) for c in transformedBytes]
    ret = ''.join(bytesToString)
    return ret
