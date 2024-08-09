#ifndef QZeroRPCGLOBAL_H
#define QZeroRPCGLOBAL_H

#define METHOD0(name, returnType) \
returnType name() { \
    return sendSync<returnType>(#name, {}); \
}

#define METHOD1(name, returnType, parameterType0) \
returnType name(const parameterType0& par0) { \
    QVariantList varlist; \
    varlist << par0; \
    return sendSync<returnType>(#name, varlist); \
}

#define METHOD2(name, returnType, parameterType0, parameterType1) \
returnType name(const parameterType0& par0, \
                const parameterType1& par1) { \
    QVariantList varlist; \
    varlist << par0 << par1; \
    return sendSync<returnType>(#name, varlist); \
}

#define METHOD3(name, returnType, parameterType0, parameterType1, parameterType2) \
returnType name(const parameterType0& par0, \
                const parameterType1& par1, \
                const parameterType2& par2) { \
    QVariantList varlist; \
    varlist << par0 << par1 << par2; \
    return sendSync<returnType>(#name, varlist); \
}

#define METHOD4(name, returnType, parameterType0, parameterType1, parameterType2, parameterType3) \
returnType name(const parameterType0& par0, \
                const parameterType1& par1, \
                const parameterType2& par2, \
                const parameterType3& par3) { \
    QVariantList varlist; \
    varlist << par0 << par1 << par2 << par3; \
    return sendSync<returnType>(#name, varlist); \
}

#define METHOD5(name, returnType, parameterType0, parameterType1, parameterType2, parameterType3, parameterType4) \
returnType name(const parameterType0& par0, \
                const parameterType1& par1, \
                const parameterType2& par2, \
                const parameterType3& par3, \
                const parameterType4& par4) { \
    QVariantList varlist; \
    varlist << par0 << par1 << par2 << par3 << par4; \
    return sendSync<returnType>(#name, varlist); \
}

#endif // QZeroRPCGLOBAL_H
