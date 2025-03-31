#include "oknoarx.h"
#include "ui_oknoarx.h"
#include <QStringList>

OknoARX::OknoARX(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoARX)
{
    ui->setupUi(this);
}

OknoARX::~OknoARX()
{
    delete ui;
}

void OknoARX::ustawDane(const std::vector<double> &a, const std::vector<double> &b, int opoznienie, double zaklocenie)
{
    QStringList a_str, b_str;
    for (double val : a)
        a_str << QString::number(val);
    for (double val : b)
        b_str << QString::number(val);
    ui->plain_a->setPlainText(a_str.join(" "));
    ui->plain_b->setPlainText(b_str.join(" "));
    ui->spinbox_opoznienie->setValue(opoznienie);
    ui->spinbox_zaklocenie->setValue(zaklocenie);
}

void OknoARX::pobierzDane(std::vector<double> &a, std::vector<double> &b, int &opoznienie, double &zaklocenie) const
{
    a.clear();
    b.clear();
    bool czyjestok;
    for (const QString &val : ui->plain_a->toPlainText().split(" ", Qt::SkipEmptyParts))
    {
        double d = val.toDouble(&czyjestok);
        if (czyjestok) a.push_back(d);
    }

    for (const QString &val : ui->plain_b->toPlainText().split(" ", Qt::SkipEmptyParts))
    {
        double d = val.toDouble(&czyjestok);
        if (czyjestok) b.push_back(d);
    }
    opoznienie = static_cast<int>(ui->spinbox_opoznienie->value());
    zaklocenie = ui->spinbox_zaklocenie->value();
}
void OknoARX::on_buttonBox_accepted()
{
    this->accepted();
}

