#include "radar_indicator.h"

#include <QGraphicsView>

#include <QtMath>

RadarIndicator::RadarIndicator(QGraphicsView* draw_area) {
    //计算绘图区域边长
    draw_area_side_ = qMin(draw_area->width(), draw_area->height());

    //初始化雷达指示器绘图默认值
    InitDefaultParameter();
}
//设置形状,即起始角度和转过角度
void RadarIndicator::SetShape(double start_angle_degree,
                              double span_angle_degree) {
    start_angle_radar_ = start_angle_degree;
    span_angle_radar_ = span_angle_degree;
    AngleInRadarToQt();
}
//设置量程,最大刻度和最小刻度
void RadarIndicator::SetRange(double range_meter,
                              double calibration_meter) {
    range_ = range_meter;
    calibration_ = calibration_meter;
    CalculateArcsParameter();
}
//设置主刻度圈数
void RadarIndicator::SetMainArcsIndex(int index) {
    main_arcs_index_ = index;
}
//设置主刻度颜色
void RadarIndicator::SetMainArcsColor(QColor color) {
    main_arcs_color_ = color;
}
//设置副刻度颜色
void RadarIndicator::SetSubArcsColor(QColor color) {
    sub_arcs_color_ = color;
}
//设置圆弧刻度文字是否可见
void RadarIndicator::SetArcsTextVisible(bool is_visible) {
    is_show_arcs_text_ = is_visible;
}
//设置刻度单位,默认km
void RadarIndicator::SetArcUnit(RadarIndicator::ArcUnit unit) {
    switch (unit) {
    case NoneUnit:
        arc_unit_ = "";
        break;
    case Meter:
        arc_unit_ = "m";
        break;
    case KiloMeter:
        arc_unit_ = "km";
        break;
    }
}
//设置刻度文字颜色
void RadarIndicator::SetArcsTextColor(QColor color) {
    arc_text_color_ = color;
}
//绑定绘图区域,控件绘图区域大小加上半个画笔宽度
QRectF RadarIndicator::boundingRect() const {
    return QRectF(-draw_area_side_ / 2 - pen_width_ / 2,
                  -draw_area_side_ / 2 - pen_width_ / 2,
                  draw_area_side_ + pen_width_ / 2,
                  draw_area_side_ + pen_width_ / 2);
}
//绘图主函数
void RadarIndicator::paint(QPainter* painter,
                           const QStyleOptionGraphicsItem* option,
                           QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    DrawArcs(painter);
    if (is_show_arcs_text_) {
        DrawArcsText(painter);
    }
}
//初始化默认绘图参数
void RadarIndicator::InitDefaultParameter() {
    pen_width_ = 1;
    DefaultRadarParameter();
    DefaultPenParameter();
}
//默认雷达参数
void RadarIndicator::DefaultRadarParameter() {
    //默认角度,雷达系下0~360度
    SetShape();
    //默认量程,1000m,每圈100m
    SetRange(1000, 100);
    //主刻度
    main_arcs_index_ = 5;
    //刻度可见
    is_show_arcs_text_ = true;
    //单位km
    SetArcUnit(Meter);
}
//默认画笔参数
void RadarIndicator::DefaultPenParameter() {
    //圆弧颜色
    main_arcs_color_ = Qt::white;
    sub_arcs_color_ = Qt::green;
    //刻度文字颜色
    arc_text_color_ = Qt::white;
}
//绘制圆弧
void RadarIndicator::DrawArcs(QPainter* painter) {
    //放缩系数
    qreal scale_factor = painter->transform().m11();
    //画笔
    QPen pen_arcs;
    //画笔宽度随放大倍数减少
    pen_arcs.setWidthF(pen_width_ / scale_factor);
    //画笔颜色
    pen_arcs.setColor(sub_arcs_color_);
    painter->setPen(pen_arcs);
    for (int i = 1; i <= radius_divide_number_; i++) {
        if (i % main_arcs_index_ == 0) {
            //主刻度换颜色
            painter->save();
            pen_arcs.setColor(main_arcs_color_);
            painter->setPen(pen_arcs);
            double temp_radius = max_radius_ * i / radius_divide_number_;
            QRectF temp_rect_arc(-temp_radius, -temp_radius,
                                 temp_radius * 2, temp_radius * 2);
            painter->drawArc(temp_rect_arc, start_angle_qt_ * 16, span_angle_qt_ * 16);
            painter->restore();
        } else {
            double temp_radius = max_radius_ * i / radius_divide_number_;
            QRectF temp_rect_arc(-temp_radius, -temp_radius,
                                 temp_radius * 2, temp_radius * 2);
            painter->drawArc(temp_rect_arc, start_angle_qt_ * 16, span_angle_qt_ * 16);
        }
    }
}
//绘制圆弧刻度
void RadarIndicator::DrawArcsText(QPainter* painter) {
    //放缩系数
    qreal scale_factor = painter->transform().m11();
    //画笔
    QPen pen_arc_text;
    //画笔宽度随放大倍数减少
    pen_arc_text.setWidthF(pen_width_ / scale_factor);
    //画笔颜色
    pen_arc_text.setColor(arc_text_color_);
    painter->setPen(pen_arc_text);
    //字体大小调整
    int font_size = 10;
    if (scale_factor >= 1) {
        font_size /= scale_factor;
        if (font_size < 2) {
            font_size = 2;
        }
    }
    QFont font("Arial", font_size);
    painter->setFont(font);
    if (span_angle_radar_ >= 360 || (start_angle_radar_ <= 90 &&
                                     start_angle_radar_ >= 90)) {
        //如果90度在雷达范围内
        for (int i = 1; i <= radius_divide_number_; i++) {
            //文字
            QString temp_text;
            if (arc_unit_ == "km") {
                temp_text = QString::number(calibration_ * i * 0.001) + arc_unit_;
            } else {
                temp_text = QString::number(calibration_ * i) + arc_unit_;
            }
            QFontMetrics text_font(font);
            //雷达系下90度位置
            QPointF temp_point_text(max_radius_ * i / radius_divide_number_,
                                    text_font.height());
            painter->drawText(temp_point_text, temp_text);
        }
    } else if ((start_angle_radar_ + span_angle_radar_) >= 360 ||
               (start_angle_radar_ < 0 && (start_angle_radar_ + span_angle_radar_) >= 0)) {
        //如果0度在雷达范围内
        for (int i = 1; i <= radius_divide_number_; i++) {
            //文字
            QString temp_text;
            if (arc_unit_ == "km") {
                temp_text = QString::number(calibration_ * i * 0.001) + arc_unit_;
            } else {
                temp_text = QString::number(calibration_ * i) + arc_unit_;
            }
            //雷达系下0度位置
            QPointF temp_point_text(0,
                                    -max_radius_ * i / radius_divide_number_);
            painter->drawText(temp_point_text, temp_text);
        }
    } else {
        //其他情况不显示刻度
        is_show_arcs_text_ = false;
    }
}
//绘制圆弧的角度从雷达坐标系转换为Qt绘图坐标系
void RadarIndicator::AngleInRadarToQt() {
    if (start_angle_radar_ < 0) {
        start_angle_qt_ = -start_angle_radar_ - 270;
    } else {
        start_angle_qt_ = -start_angle_radar_ + 90;
    }
    span_angle_qt_ = -span_angle_radar_;
}
//计算圆弧最大半径,圈数
void RadarIndicator::CalculateArcsParameter() {
    max_radius_ = (draw_area_side_ - 70) * 0.5 ;
    radius_divide_number_ = range_ / calibration_;
}
