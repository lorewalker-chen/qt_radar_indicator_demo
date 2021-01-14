#ifndef RADARINDICATOR_H
#define RADARINDICATOR_H

#include <QGraphicsItem>

class QGraphicsView;
class RadarIndicator: public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
  public:
    enum ArcUnit {
        NoneUnit,
        Meter,
        KiloMeter
    };
    explicit RadarIndicator(QGraphicsView* draw_area);
    //圆弧
    void SetShape(double start_angle_degree = 0, double span_angle_degree = 360);
    void SetRange(double range_meter = 1000, double calibration_meter = 100);
    void SetMainArcsIndex(int index);
    void SetMainArcsColor(QColor color);
    void SetSubArcsColor(QColor color);
    //距离刻度
    void SetArcsTextVisible(bool is_visible);
    void SetArcUnit(ArcUnit unit = ArcUnit::KiloMeter);
    void SetArcsTextColor(QColor color);

  protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;

  private:
    void InitDefaultParameter();
    void DefaultRadarParameter();
    void DefaultPenParameter();

    void DrawArcs(QPainter* painter);
    void DrawArcsText(QPainter* painter);

    void AngleInRadarToQt();
    void CalculateArcsParameter();

  private:
    //实际参数
    double start_angle_radar_;//雷达坐标系下起始角度
    double span_angle_radar_;//雷达坐标系下转过角度
    double range_;//量程
    double calibration_;//刻度
    //绘图参数
    int draw_area_side_;//绘图区域(正方形)边长
    double pen_width_;//画笔宽度

    //画圆弧
    int start_angle_qt_;//qt绘图坐标系下起始角度
    int span_angle_qt_;//qt绘图坐标系下转过角度
    double max_radius_;//最大半径
    int radius_divide_number_;//圈数
    int main_arcs_index_;//主刻度圈序号,默认为5的倍数
    QColor main_arcs_color_;//主刻度颜色
    QColor sub_arcs_color_;//副刻度颜色
    //画圆弧刻度文字
    bool is_show_arcs_text_;//刻度是否可见
    QString arc_unit_;//单位
    QColor arc_text_color_;//刻度文字颜色
};

#endif // RADARINDICATOR_H
