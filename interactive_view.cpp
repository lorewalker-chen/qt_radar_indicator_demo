#include "interactive_view.h"

#include <QWheelEvent>
#include <QKeyEvent>

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

InteractiveView::InteractiveView(QWidget* parent)
    : QGraphicsView(parent),
      translate_button_(Qt::LeftButton),
      scale_(1.0),
      zoom_delta_(0.1),
      translate_speed_(1.0),
      is_translate_(false) {
    //去掉滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //鼠标形状改为手型
    setCursor(Qt::PointingHandCursor);
    //平滑
    setRenderHint(QPainter::Antialiasing, false);
    //设置场景矩形
    setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
    //设置视图中心
    centerOn(0, 0);
    //设置缓冲模式为背景模式
    setCacheMode(QGraphicsView::CacheBackground);
    //设置每次刷新不保存绘图设备状态
    setOptimizationFlag(QGraphicsView::DontSavePainterState);
    //设置刷新模式
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}
//设置平移速度
void InteractiveView::SetTranslateSpeed(qreal speed) {
    //建议速度范围
    Q_ASSERT_X(speed >= 0.0 && speed <= 2.0,
               "InteractiveView::SetTranslateSpeed",
               "Speed should be in range [0.0, 2.0].");
    translate_speed_ = speed;
}
//获取当前平移速度
qreal InteractiveView::TranslateSpeed() const {
    return translate_speed_;
}
//设置缩放增量
void InteractiveView::SetZoomDelta(qreal delta) {
    //建议增量范围
    Q_ASSERT_X(delta >= 0.0 && delta <= 1.0,
               "InteractiveView::SetZoomDelta",
               "Speed should be in range [0.0, 1.0].");
    zoom_delta_ = delta;
}
//获取当前缩放增量
qreal InteractiveView::ZoomDelta() const {
    return zoom_delta_;
}
//鼠标移动事件，获取移动中鼠标位置
void InteractiveView::mouseMoveEvent(QMouseEvent* event) {
    if (is_translate_) {
        QPointF mouse_delta = mapToScene(event->pos()) - mapToScene(mouse_last_pos_);
        Translate(mouse_delta);
    }
    mouse_last_pos_ = event->pos();
    QGraphicsView::mouseMoveEvent(event);
}
//鼠标按下事件，开始平移，获取按下位置
void InteractiveView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == translate_button_) {
        is_translate_ = true;
        mouse_last_pos_ = event->pos();
    }
    QGraphicsView::mousePressEvent(event);
}
//鼠标释放事件，停止平移
void InteractiveView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == translate_button_) {
        is_translate_ = false;
    }
    QGraphicsView::mouseReleaseEvent(event);
}
//鼠标滚轮事件，缩放
void InteractiveView::wheelEvent(QWheelEvent* event) {
    //滚轮的滚动量
    QPoint scroll_amount = event->angleDelta();
    //正值表示滚轮远离使用者（放大），负值表示朝向使用者（缩小）
    scroll_amount.y() > 0 ? ZoomIn() : ZoomOut();
}
//放大
void InteractiveView::ZoomIn() {
    Zoom(1 + zoom_delta_);
}
//缩小
void InteractiveView::ZoomOut() {
    Zoom(1 - zoom_delta_);
}
//根据比例因子scale_factor缩放
void InteractiveView::Zoom(float scale_factor) {
    //防止过小或过大
    qreal factor = transform().scale(scale_factor, scale_factor)
                   .mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100) {
        return;
    }
    scale(scale_factor, scale_factor);
    scale_ *= scale_factor;
}
//平移
void InteractiveView::Translate(QPointF delta) {
    //根据当前缩放平移数
    delta *= scale_;
    delta *= translate_speed_;
    //根据鼠标按下的点作为锚点定位scene
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint new_center(VIEW_WIDTH / 2 - delta.x(), VIEW_HEIGHT / 2 - delta.y());
    centerOn(mapToScene(new_center));
    //scene在view的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}
