#ifndef ADMIN_H
#define ADMIN_H

#ifdef __cplusplus
extern "C" {
#endif

int adminLogin(void);
void adminLogout(void);
int isAdminLoggedIn(void);

#ifdef __cplusplus
}
#endif

#endif