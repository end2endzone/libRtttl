#pragma once

void showAssertionErrorMessage(const char * iAssertionType, const char * iExpr, const char * iSubError, const char * iFile, int iLine);

#define ASSERT_TRUE(expr) {if (!(expr)) showAssertionErrorMessage("ASSERT_TRUE", #expr, "", __FILE__, __LINE__);}
#define ASSERT_FALSE(expr) {if ((expr)) showAssertionErrorMessage("ASSERT_FALSE", #expr, "", __FILE__, __LINE__);}
#define ASSERT_NEAR(actual, expected, epsilon) {if (  !(   (actual-epsilon) <= expected && expected <= (actual+epsilon)   )) { char expr[10240]; char subError[10240]; sprintf(expr,"%s, %s, %s", #actual, #expected, #epsilon); sprintf(subError,"where: \n%s evaluates to %f\n%s evaluates to %f and\n%s evaluates to %f\n", #actual, actual, #expected, expected, #epsilon, epsilon); showAssertionErrorMessage("ASSERT_NEAR", expr, subError, __FILE__, __LINE__);}}
