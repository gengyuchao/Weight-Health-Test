#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QMessageBox"

#include "QPainter"


//BMR的计算公式
//男性：BMR=10 X 体重（kg） + 6.25 X 身高(cm) - 5 X 年龄 + 5
//女性：BMR=10 X 体重（kg） + 6.25 X 身高（cm） - 5 x 年龄 - 161


class Human_body_attribute
{

public:

    typedef enum {
        MALE = 0,
        FEMALE = 1
    } gender_type;

    typedef enum {
        Thin,
        Normal,
        Overweight,
        Obesity,
    } body_shape_type;

    int gender = MALE;
    float weight=0;
    float height=0;
    float age=0;
    float BMI=0;
    float BMR=0;
    float REE=0;
    float Body_fat_rate=0;

    body_shape_type shape;

    void calculate_BMI();
    void calculate_BMR();
    void calculate_Body_fat_rate();
    bool Data_valid();

private:

};

bool Human_body_attribute::Data_valid()
{
    if(weight==0||height==0||age==0)
        return true;
    else
        return false;
}

//BMI计算法：
//   BMI=体重（公斤）÷（身高×身高）（米）


void Human_body_attribute::calculate_BMI()
{
    if(Data_valid()==true)
    {
       QMessageBox::warning(NULL, "错误", "数据无效！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return ;
    }

    BMI = weight / (height*height);

}

//男性：REE=10 X 体重（kg） + 6.25 X 身高(cm) - 5 X 年龄 + 5
//女性：REE=10 X 体重（kg） + 6.25 X 身高（cm） - 5 x 年龄 - 161

//基礎代謝率(BMR)是指：我們在安靜狀態下(通常為靜臥狀態)消耗的最低熱量，人的其他活動都建立在這個基礎上。那麼，如何計算自己的基礎代謝率呢？美國運動醫學協會提供了以下一個公式：
//BMR(男)=(13.7×體重(公斤))+(5.0×身高(公分))-(6.8×年齡)+66
//BMR(女)=(9.6×體重(公斤))+(1.8×身高(公分))-(4.7×年齡)+655

//卡路里和营养元素（脂肪，蛋白质，碳水化合物，微量元素）
//1g的碳水化合 = 4卡路里
//1g的蛋白质 = 4卡路里
//1g的脂肪 = 9卡路里
//建议：脂肪，蛋白质，碳水化合物的比例
//脂肪应占BMR的15%-25%之间。蛋白质应占体重的2倍，省下的热量应由碳水化合物来补充。

void Human_body_attribute::calculate_BMR()
{
    if(Data_valid()==true)
    {
       QMessageBox::warning(NULL, "错误", "数据无效！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return ;
    }
    if(gender == MALE)
    {
        BMR = 13.7*weight + 500.0*height - 6.8*age +66.0;
        REE = 10.0 * weight + 625 * height - 5.0 * age +5.0 ;
    }
    else
    {
        BMR = 9.6*weight + 180.0*height - 4.7*age + 655;
        REE = 10.0 * weight + 625.0 * height - 5.0 * age -161.0 ;
    }
}

//　　体脂率可通过BMI计算法计算得出。
//    体脂率：1.2×BMI+0.23×年龄-5.4-10.8×性别（男为1，女为0）

void Human_body_attribute::calculate_Body_fat_rate()
{
    if(Data_valid()==true)
    {
       QMessageBox::warning(NULL, "错误", "数据无效！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return ;
    }
    if(gender == MALE)
    {
        Body_fat_rate = 1.2 * BMI + 0.23 * age - 16.2;

        if(Body_fat_rate<10)
            shape = Thin;
        else if(Body_fat_rate<21)
            shape = Normal;
        else if(Body_fat_rate<26)
            shape = Overweight;
        else if(Body_fat_rate<45)
            shape = Obesity;
        else
            shape = Obesity;
    }
    else
    {
        Body_fat_rate = 1.2 * BMI + 0.23 * age - 5.4;

        if(Body_fat_rate<20)
            shape = Thin;
        else if(Body_fat_rate<34)
            shape = Normal;
        else if(Body_fat_rate<39)
            shape = Overweight;
        else if(Body_fat_rate<45)
            shape = Obesity;
        else
            shape = Obesity;
    }


}

Human_body_attribute my_body;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Btn_Set_clicked()
{
    my_body.weight = ui->lineEdit_body_weight->text().toFloat();
    my_body.height = ui->lineEdit_body_height->text().toFloat();
    my_body.age = ui->lineEdit_body_age->text().toFloat();
    my_body.gender = ui->radioButton_male->isChecked()?Human_body_attribute::MALE:Human_body_attribute::FEMALE;


    my_body.calculate_BMI();
    my_body.calculate_Body_fat_rate();
    my_body.calculate_BMR();

    QString result_info;

    result_info.sprintf("\n性别:\t\t\t%s \n身体质量指数(BMI):\t%3.2f \n体脂率:\t\t\t%3.2f \n静态能量消耗值(REE):\t%f \n基础代谢率(BMR):\t%f \n",
                        my_body.gender==Human_body_attribute::MALE?"男":"女",my_body.BMI,my_body.Body_fat_rate,my_body.REE,my_body.BMR);
    result_info.append("");
    ui->label_result->setText(result_info);
    this->update();

}

#define Frame_Length 72
#define Frame_Width 39

#define Frame_START_Offset_x 153
#define Frame_START_Offset_y 9

uint8_t age_Group[3]=
{
    17,39,59
};

int offset_x=Frame_START_Offset_x,offset_y=Frame_START_Offset_y;

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setPen(QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap,
                        Qt::MiterJoin));

    QRectF target(421.0, 21.0, 461.0, 341.0);
    painter.drawImage(target,QImage(":/对照表.png"));

    offset_x = Frame_START_Offset_x + (Frame_Length+3)*my_body.shape;
    offset_y = Frame_START_Offset_y+ (!my_body.gender)*145
            +(my_body.age>age_Group[1])*50
            +(my_body.age>age_Group[2])*50
            ;

    QPoint set_position(421.0+offset_x,21.0+offset_y);

    if(my_body.Data_valid()==false)
    {
        painter.drawLine(set_position,QPoint(set_position.x()+Frame_Length,set_position.y()));
        painter.drawLine(set_position,QPoint(set_position.x(),set_position.y()+Frame_Width));
        painter.drawLine(QPoint(set_position.x(),set_position.y()+Frame_Width),QPoint(set_position.x()+Frame_Length,set_position.y()+Frame_Width));
        painter.drawLine(QPoint(set_position.x()+Frame_Length,set_position.y()),QPoint(set_position.x()+Frame_Length,set_position.y()+Frame_Width));
    }
}

