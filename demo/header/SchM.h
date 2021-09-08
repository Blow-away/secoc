#ifndef __SchM__
#define __SchM__

//0x03
//此函数执行SecOC模块对Tx路径的身份验证和验证处理过程。
//SecOC_MainFunctionTx的周期时间依据参数SecOCMainFunctionPeriodTx进行配置
//如果已经安排了SecOC_MainFunctionTx，SecOC首先要检查是否有新的Authentic I-PDU需要认证。
//如果有，SecOC模块则将进行每个IPDU的身份验证，
//随后将这些IPDU在相同的主函数调用中识别为新的
//对于每个新认证的Authentic I-PDU, SecOC模块应立即通过调用PduR触发底层模块的Secured I-PDU传输
void SecOC_MainFunctionTx(void);

#endif