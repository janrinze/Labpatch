#ifndef DEFINES_H
#define DEFINES_H

#include <QBrush>
#include <QString>
#include <QList>

#include <functional>

#define BLOCKDIAGMIME "application/x-labpatchitemdata"
#define FIXEDICONHEIGHT 54
#define FIXEDICONHEIGHTSPACING 11
#define FIXEDICONWIDTH 54
#define FIXEDICONWIDTHSPACING 11
#define FIXEDMOUSEICONHEIGHT 38
#define FIXEDMOUSEICONWIDTH 26

#define LPH_BOOLTYPE            0x00000001
#define LPH_INTTYPE             0x00000002
#define LPH_DOUBLETYPE          0x00000004
#define LPH_COMPLEXTYPE         0x00000008
#define LPH_STRINGTYPE          0x00000010
#define LPH_FILETYPE            0x00000020

const static QMap<QString, QBrush> iconcolordatabase{
    {QString("LPH_BOOLTYPE") , QColor(0x00, 0x66, 0x00)},
    {QString("LPH_INTTYPE") , QColor(0x00, 0x00, 0x99)},
    {QString("LPH_DOUBLETYPE") , QColor(0xFF, 0x60, 0x00)},
    {QString("LPH_COMPLEXTYPE") , QColor(0x99, 0x00, 0x66)},
    {QString("LPH_STRINGTYPE") , QColor(0x9A, 0x00, 0x00)},
    {QString("LPH_FILETYPE") , Qt::darkGreen},
    {QString("LPH_UNKNOWN") , QColor(0x63, 0x00, 0xFF)},
};

//Structures, Numeric, Boolean, String, Array, ???, Comparison, Timing,
const static QList<QList<QList<QList<QString> > > > icondatabase =
{
    {
        {
            {":/images/menu01.png", "Inputs and Outputs"}
        },
        {
            {":/images/icon_bool_input.png", "Boolean Input", "54", "54", "boolInput", "edit:(bool_QCheckBox_false),out:(LPH_BOOLTYPE){0|0|53|53|53|27}"},
            {":/images/icon_bool_output.png", "Boolean Output", "54", "54", "boolOutput", "in:(LPH_BOOLTYPE){0|0|53|53|0|27},term:(bool_QCheckBox_false)"},
            {":/images/icon_int_input.png", "Integer Input", "54", "54", "intInput", "edit:(int_QSpinBox_0),out:(LPH_INTTYPE){0|0|53|53|53|27}"},
            {":/images/icon_int_output.png", "Integer Output", "54", "54", "intOutput", "in:(LPH_INTTYPE){0|0|53|53|0|27},term:(int_QSpinBox_0)"},
            {":/images/icon_double_input.png", "Double Input", "54", "54", "doubleInput", "edit:(double_QDoubleSpinBox_0.0),out:(LPH_DOUBLETYPE){0|0|53|53|53|27}"},
            {":/images/icon_double_output.png", "Double Output", "54", "54", "doubleOutput", "in:(LPH_DOUBLETYPE){0|0|53|53|0|27},term:(double_QDoubleSpinBox_0.0)"},
            {":/images/icon_complex_input.png", "Complex Input", "54", "54", "complexInput", "edit:(real_QDoubleSpinBox_0.0;img_QDoubleSpinBox_0.0),out:(LPH_COMPLEXTYPE){0|0|53|53|53|27}"},
            {":/images/icon_complex_output.png", "Complex Output", "54", "54", "complexOutput", "in:(LPH_COMPLEXTYPE){0|0|53|53|0|27},term:(real_QDoubleSpinBox_0.0;img_QDoubleSpinBox_0.0)"},
            {":/images/icon_cast_int2double.png", "Cast Integer to Double", "54", "54", "int2doubleCast", "in:(LPH_INTTYPE){0|0|27|53|0|27},out:(LPH_DOUBLETYPE){28|0|53|53|53|27}"},
            {":/images/icon_cast_double2complex.png", "Cast Double to Complex", "54", "54", "double2complexCast", "in:(LPH_DOUBLETYPE){0|0|27|53|0|27},out:(LPH_COMPLEXTYPE){28|0|53|53|53|27}"},
        }
    },
    {
        {
            {":/images/menu02.png", "Functions"}
        },
        {
            {":/images/icon_func_plus.png", "Addition", "54", "54", "plusFunc", "in:(LPH_INTTYPE|LPH_DOUBLETYPE|LPH_COMPLEXTYPE){0|0|27|27|0|17},in:(LPH_INTTYPE|LPH_DOUBLETYPE|LPH_COMPLEXTYPE){0|28|27|53|0|38},out:(LPH_INTTYPE|LPH_DOUBLETYPE|LPH_COMPLEXTYPE){28|0|53|53|53|27}", "0:LPH_INTTYPE{0>LPH_INTTYPE;1>LPH_INTTYPE;2>LPH_INTTYPE}|LPH_DOUBLETYPE{0>LPH_DOUBLETYPE;1>LPH_DOUBLETYPE;2>LPH_DOUBLETYPE}|LPH_COMPLEXTYPE{0>LPH_COMPLEXTYPE;1>LPH_COMPLEXTYPE;2>LPH_COMPLEXTYPE},1:LPH_INTTYPE{0>LPH_INTTYPE;1>LPH_INTTYPE;2>LPH_INTTYPE}|LPH_DOUBLETYPE{0>LPH_DOUBLETYPE;1>LPH_DOUBLETYPE;2>LPH_DOUBLETYPE}|LPH_COMPLEXTYPE{0>LPH_COMPLEXTYPE;1>LPH_COMPLEXTYPE;2>LPH_COMPLEXTYPE},2:LPH_INTTYPE{0>LPH_INTTYPE;1>LPH_INTTYPE;2>LPH_INTTYPE}|LPH_DOUBLETYPE{0>LPH_DOUBLETYPE;1>LPH_DOUBLETYPE;2>LPH_DOUBLETYPE}|LPH_COMPLEXTYPE{0>LPH_COMPLEXTYPE;1>LPH_COMPLEXTYPE;2>LPH_COMPLEXTYPE}"},
            {":/images/icon_func_minus.png", "Subtraction", "54", "54", "minusFunc", "in:(LPH_INTTYPE|LPH_DOUBLETYPE|LPH_COMPLEXTYPE){0|0|27|27|0|17},in:(LPH_INTTYPE|LPH_DOUBLETYPE|LPH_COMPLEXTYPE){0|28|27|53|0|38},out:(LPH_INTTYPE|LPH_DOUBLETYPE|LPH_COMPLEXTYPE){28|0|53|53|53|27}", "0:LPH_INTTYPE{0>LPH_INTTYPE;1>LPH_INTTYPE;2>LPH_INTTYPE}|LPH_DOUBLETYPE{0>LPH_DOUBLETYPE;1>LPH_DOUBLETYPE;2>LPH_DOUBLETYPE}|LPH_COMPLEXTYPE{0>LPH_COMPLEXTYPE;1>LPH_COMPLEXTYPE;2>LPH_COMPLEXTYPE},1:LPH_INTTYPE{0>LPH_INTTYPE;1>LPH_INTTYPE;2>LPH_INTTYPE}|LPH_DOUBLETYPE{0>LPH_DOUBLETYPE;1>LPH_DOUBLETYPE;2>LPH_DOUBLETYPE}|LPH_COMPLEXTYPE{0>LPH_COMPLEXTYPE;1>LPH_COMPLEXTYPE;2>LPH_COMPLEXTYPE},2:LPH_INTTYPE{0>LPH_INTTYPE;1>LPH_INTTYPE;2>LPH_INTTYPE}|LPH_DOUBLETYPE{0>LPH_DOUBLETYPE;1>LPH_DOUBLETYPE;2>LPH_DOUBLETYPE}|LPH_COMPLEXTYPE{0>LPH_COMPLEXTYPE;1>LPH_COMPLEXTYPE;2>LPH_COMPLEXTYPE}"},
            {":/images/icon_func_mul.png", "Multiplication", "54", "54", "mulFunc", "in:(LPH_INTTYPE|LPH_DOUBLETYPE|LPH_COMPLEXTYPE){0|0|27|27|0|17},in:(LPH_INTTYPE|LPH_DOUBLETYPE|LPH_COMPLEXTYPE){0|28|27|53|0|38},out:(LPH_INTTYPE|LPH_DOUBLETYPE|LPH_COMPLEXTYPE){28|0|53|53|53|27}", "0:LPH_INTTYPE{0>LPH_INTTYPE;1>LPH_INTTYPE;2>LPH_INTTYPE}|LPH_DOUBLETYPE{0>LPH_DOUBLETYPE;1>LPH_DOUBLETYPE;2>LPH_DOUBLETYPE}|LPH_COMPLEXTYPE{0>LPH_COMPLEXTYPE;1>LPH_COMPLEXTYPE;2>LPH_COMPLEXTYPE},1:LPH_INTTYPE{0>LPH_INTTYPE;1>LPH_INTTYPE;2>LPH_INTTYPE}|LPH_DOUBLETYPE{0>LPH_DOUBLETYPE;1>LPH_DOUBLETYPE;2>LPH_DOUBLETYPE}|LPH_COMPLEXTYPE{0>LPH_COMPLEXTYPE;1>LPH_COMPLEXTYPE;2>LPH_COMPLEXTYPE},2:LPH_INTTYPE{0>LPH_INTTYPE;1>LPH_INTTYPE;2>LPH_INTTYPE}|LPH_DOUBLETYPE{0>LPH_DOUBLETYPE;1>LPH_DOUBLETYPE;2>LPH_DOUBLETYPE}|LPH_COMPLEXTYPE{0>LPH_COMPLEXTYPE;1>LPH_COMPLEXTYPE;2>LPH_COMPLEXTYPE}"},
            {":/images/icon_func_div.png", "Division", "54", "54", "divFunc", "in:(LPH_INTTYPE|LPH_DOUBLETYPE|LPH_COMPLEXTYPE){0|0|27|27|0|17},in:(LPH_INTTYPE|LPH_DOUBLETYPE|LPH_COMPLEXTYPE){0|28|27|53|0|38},out:(LPH_DOUBLETYPE|LPH_COMPLEXTYPE){28|0|53|53|53|27}","0:LPH_INTTYPE{0>LPH_INTTYPE;1>LPH_INTTYPE;2>LPH_DOUBLETYPE}|LPH_DOUBLETYPE{0>LPH_DOUBLETYPE;1>LPH_DOUBLETYPE;2>LPH_DOUBLETYPE}|LPH_COMPLEXTYPE{0>LPH_COMPLEXTYPE;1>LPH_COMPLEXTYPE;2>LPH_COMPLEXTYPE},1:LPH_INTTYPE{0>LPH_INTTYPE;1>LPH_INTTYPE;2>LPH_DOUBLETYPE}|LPH_DOUBLETYPE{0>LPH_DOUBLETYPE;1>LPH_DOUBLETYPE;2>LPH_DOUBLETYPE}|LPH_COMPLEXTYPE{0>LPH_COMPLEXTYPE;1>LPH_COMPLEXTYPE;2>LPH_COMPLEXTYPE},"},
            {":/images/icon_func_and.png", "Boolean And", "54", "54", "andFunc", "in:(LPH_BOOLTYPE){0|0|27|27|0|17},in:(LPH_BOOLTYPE){0|28|27|53|0|38},out:(LPH_BOOLTYPE){28|0|53|53|53|27}"},
            {":/images/icon_func_or.png", "Boolean Or", "54", "54", "orFunc", "in:(LPH_BOOLTYPE){0|0|27|27|0|17},in:(LPH_BOOLTYPE){0|28|27|54|0|38},out:(LPH_BOOLTYPE){28|0|53|53|53|27}"},
            {":/images/icon_func_not.png", "Boolean Not", "54", "54", "notFunc", "in:(LPH_BOOLTYPE){0|0|27|53|0|27},out:(LPH_BOOLTYPE){28|0|53|53|53|27}"},
            {"://images/icon_func_xor.png", "Boolean Exclusive Or", "54", "54", "xorFunc", "in:(LPH_BOOLTYPE){0|0|27|27|0|17},in:(LPH_BOOLTYPE){0|28|27|53|0|38},out:(LPH_BOOLTYPE){28|0|53|53|53|27}"},
        }
    },
    {
        {
            {":/images/menu03.png", "Strings"}
        },
        {
            {":/images/icon_str_constant.png", "String Input", "54", "54", "constStr", "edit:(string_QLineEdit_empty),out:(LPH_STRINGTYPE){0|0|53|53|53|27}"},
            {":/images/icon_str_cat.png", "String Concatination", "54", "54", "catStr", "in:(LPH_STRINGTYPE){0|0|27|27|0|17},in:(LPH_STRINGTYPE){0|28|27|53|0|38},out:(LPH_STRINGTYPE){28|0|53|53|53|27}"},
            {":/images/icon_str_out.png", "String Output", "54", "54", "outStr", "in:(LPH_STRINGTYPE){0|0|53|53|0|27},term:(string_QLineEdit_empty)"},
        }
    },
};

//This is for programming each icon's functionality
//A drawback: the function type has to be determined by the type at subindex 0. sorry.
const static QMap<QString, std::function<void(QString type, QList<QList<QByteArray> > &i)> > iconfuncdatabase = {
{"plusFunc", [](QString type, QList<QList<QByteArray> > &i){
        if(type == "LPH_INTTYPE"){
            i[2] = QList<QByteArray>({QByteArray::number(i[0][0].toInt() + i[1][0].toInt())});
        }else if(type == "LPH_DOUBLETYPE"){
            i[2] = QList<QByteArray>({QByteArray::number(i[0][0].toDouble() + i[1][0].toDouble())});
        }else if(type == "LPH_COMPLEXTYPE"){
            i[2] = QList<QByteArray>({
                 QByteArray::number(i[0][0].toDouble() + i[1][0].toDouble()),
                 QByteArray::number(i[0][1].toDouble() + i[1][1].toDouble()),
            });
        }
    }},

{"minusFunc", [](QString type, QList<QList<QByteArray> > &i){
        if(type == "LPH_INTTYPE"){
            i[2] = QList<QByteArray>({QByteArray::number(i[0][0].toInt() - i[1][0].toInt())});
        }else if(type == "LPH_DOUBLETYPE"){
            i[2] = QList<QByteArray>({QByteArray::number(i[0][0].toDouble() - i[1][0].toDouble())});
        }else if(type == "LPH_COMPLEXTYPE"){
            i[2] = QList<QByteArray>({
                 QByteArray::number(i[0][0].toDouble() - i[1][0].toDouble()),
                 QByteArray::number(i[0][1].toDouble() - i[1][1].toDouble()),
            });
        }
    }},
{"mulFunc", [](QString type, QList<QList<QByteArray> > &i){
        if(type == "LPH_INTTYPE"){
            i[2] = QList<QByteArray>({QByteArray::number(i[0][0].toInt() * i[1][0].toInt())});
        }else if(type == "LPH_DOUBLETYPE"){
            i[2] = QList<QByteArray>({QByteArray::number(i[0][0].toDouble() * i[1][0].toDouble())});
        }else if(type == "LPH_COMPLEXTYPE"){
            i[2] = QList<QByteArray>({
                 QByteArray::number(i[0][0].toDouble()*i[0][1].toDouble() + i[1][0].toDouble()*i[1][1].toDouble()),
                 QByteArray::number(i[0][0].toDouble()*i[1][1].toDouble() + i[0][1].toDouble()*i[1][0].toDouble()),
            });
        }
    }},
{"divFunc", [](QString type, QList<QList<QByteArray> > &i){
        if(type == "LPH_INTTYPE" || type == "LPH_DOUBLETYPE"){
            i[2] = QList<QByteArray>({QByteArray::number(i[0][0].toDouble() / i[1][0].toDouble())});
        }else if(type == "LPH_COMPLEXTYPE"){
            i[2] = QList<QByteArray>({
                 QByteArray::number(i[0][0].toDouble()*i[0][1].toDouble() + i[1][0].toDouble()*i[1][1].toDouble()),
                 QByteArray::number(i[0][0].toDouble()*i[1][1].toDouble() + i[0][1].toDouble()*i[1][0].toDouble()),
            });
        }
    }},
{"int2doubleCast", [](QString type, QList<QList<QByteArray> > &i){
        (void)(type);
        i[1] = QList<QByteArray>({QByteArray::number(i[0][0].toDouble())});
    }},
{"double2complexCast", [](QString type, QList<QList<QByteArray> > &i){
        (void)(type);
        i[1] = QList<QByteArray>({QByteArray::number(i[0][0].toDouble())});
    }},
{"andFunc", [](QString type, QList<QList<QByteArray> > &i){
        (void)(type);
        if(i[0][0].contains("true") & i[1][0].contains("true"))
            i[2] = QList<QByteArray>({QByteArray("true")});
        else
            i[2] = QList<QByteArray>({QByteArray("false")});
    }},
{"orFunc", [](QString type, QList<QList<QByteArray> > &i){
        (void)(type);
        if(i[0][0].contains("true") | i[1][0].contains("true"))
            i[2] = QList<QByteArray>({QByteArray("true")});
        else
            i[2] = QList<QByteArray>({QByteArray("false")});
    }},
{"notFunc", [](QString type, QList<QList<QByteArray> > &i){
        (void)(type);
        if(i[0][0].contains("true"))
            i[1] = QList<QByteArray>({QByteArray("false")});
        else
            i[1] = QList<QByteArray>({QByteArray("true")});
    }},
{"xorFunc", [](QString type, QList<QList<QByteArray> > &i){
        (void)(type);
        if(i[0][0].contains("true") == i[1][0].contains("true"))
            i[2] = QList<QByteArray>({QByteArray("false")});
        else
            i[2] = QList<QByteArray>({QByteArray("true")});
    }},
{"catStr", [](QString type, QList<QList<QByteArray> > &i){
        (void)(type);
        i[2] = QList<QByteArray>({QByteArray(i[0][0])+QByteArray(i[1][0])});
    }},
};


#endif // DEFINES_H

