#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include <QDialog>
#include "../model/models.h"
#include "../controller/authcontroller.h"

class QLineEdit;

class ProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileDialog(User& currentUser, QWidget* parent = nullptr);

private slots:
    void onChangeLogin();
    void onChangePassword();

private:
    User& m_user;
    AuthController m_auth;

    QLineEdit* m_newLoginEdit;
    QLineEdit* m_oldPasswordEdit;
    QLineEdit* m_newPasswordEdit;
};

#endif // PROFILEDIALOG_H
