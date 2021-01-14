/*
 * 参考于https://blog.csdn.net/liang19890820/article/details/53543017
 * 创建时间:2020-12-25
 * 创建者:cgz
 * 描述:
 * 该类实现交互式的QGraphicsView.
 * 通过按下鼠标左键并拖动鼠标进行视口的平移操作,
 * 通过鼠标滚轮进行视口的缩放操作,
 * 可以自行设置平移速度和缩放增量.
 *
 * 使用方法:
 * 在qt的ui文件中,将widget控件提升为InteractiveView
 *
 * 备注:删除了键盘对视口的操作
 */

#ifndef INTERACTIVEVIEW_H
#define INTERACTIVEVIEW_H

#include <QGraphicsView>

class QWheelEvent;
class QKeyEvent;

class InteractiveView: public QGraphicsView {
    Q_OBJECT
  public:
    explicit InteractiveView(QWidget* parent = 0);

    //平移速度
    void SetTranslateSpeed(qreal speed);
    qreal TranslateSpeed() const;
    //缩放增量
    void SetZoomDelta(qreal delta);
    qreal ZoomDelta() const;

  protected:
    //平移
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    //缩放
    void wheelEvent(QWheelEvent* event) override;

  public Q_SLOTS:
    void ZoomIn();//放大
    void ZoomOut();//缩小
    void Zoom(float scale_factor);//按比例缩放
    void Translate(QPointF delta);//平移

  private:
    Qt::MouseButton translate_button_;//平移按钮
    qreal scale_;//缩放值
    qreal zoom_delta_;//缩放的增量
    qreal translate_speed_;//平移速度
    bool is_translate_;//平移标识
    QPoint mouse_last_pos_;//鼠标最后按下的位置

};

#endif // INTERACTIVEVIEW_H
