#include "gui/email/SwitchControl.hpp"
#include "gui/base/m_widget.hpp"

#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QDebug>

namespace coremail {
    namespace client_app {
        namespace gui {
            namespace email {
                std::once_flag	SwitchControl::type_register_flag;

                SwitchControl::SwitchControl(const QString& atext, const BOOL& b, QWidget* parent)
                    : QWidget(parent),
                    m_nHeight(as_96dpi(20)),
                    m_bChecked(false),
                    m_radius(as_96dpi(15.0)),
                    m_nMargin(as_96dpi(0)),
                    m_checkbackground(0, 198, 150),
                    m_uncheckbackground(249, 249, 249),
                    m_thumbColor(255, 255, 255),
                    m_disabledColor(190, 190, 190),
                    m_background(Qt::black),
                    m_text(atext),
                    m_bCheckStatus(b)
                {

                    std::call_once(type_register_flag, []() {
                        qRegisterMetaType<SwitchControl_Enum>();
                        });
  
                    setCursor(Qt::PointingHandCursor);
                    setMinimumSize(as_96dpi(50), as_96dpi(30));
                    setMaximumSize(as_96dpi(50), as_96dpi(30));
                    setToggle(b);

                    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
                }

                void SwitchControl::paintEvent(QPaintEvent* event)
                {
                    Q_UNUSED(event);

                    QPainter painter(this);
                    painter.setPen(Qt::NoPen);
                    painter.setRenderHint(QPainter::Antialiasing);

                    QPainterPath path;
                    QPainterPath pathBorder;
                    QColor background;
                    QColor thumbColor;
                    qreal dOpacity;
                    if (isEnabled()) {
                        if (m_bChecked) { 
                            background = m_checkbackground;
                            thumbColor = m_thumbColor;
                            dOpacity = 1.000;
                        }
                        else { 
                            background = m_uncheckbackground;
                            thumbColor = m_thumbColor;
                            dOpacity = 1.000;
                        }
                    }
                    else {
                        background = m_background;
                        dOpacity = 0.260;
                        thumbColor = m_disabledColor;
                    }



                    painter.setBrush(background);
                    painter.setOpacity(dOpacity);
                    path.addRoundedRect(QRectF(m_nMargin, m_nMargin, width() - 2 * m_nMargin, height() - 2 * m_nMargin), m_radius, m_radius);
                    pathBorder.addRoundedRect(QRectF(m_nMargin, m_nMargin, width() - 2 * m_nMargin, height() - 2 * m_nMargin), m_radius, m_radius);
                    painter.drawPath(path.simplified());
                    painter.setPen(QPen(QColor(223,223,223)));
                    painter.drawPath(pathBorder.simplified());


                    painter.setBrush(thumbColor);
                    painter.setOpacity(1.0);
                    painter.drawEllipse(QRectF(m_nX - (m_nHeight / 2) + 1, m_nY - (m_nHeight / 2), height(), height()));
                }
                
                void SwitchControl::mousePressEvent(QMouseEvent* event)
                {
                    if (isEnabled()) {
                        if (event->buttons() & Qt::LeftButton) {
                            event->accept();
                        }
                        else {
                            event->ignore();
                        }
                    }
                }

                void SwitchControl::mouseReleaseEvent(QMouseEvent* event)
                {
                    if (isEnabled()) {
                        if ((event->type() == QMouseEvent::MouseButtonRelease) && (event->button() == Qt::LeftButton)) {
                            event->accept();
                            m_bChecked = !m_bChecked;
                            if (m_text == tr("收取垃圾邮件"))
                            {
                                emit toggled(m_bChecked, email::SwitchControl_Enum::ReceiveSpam);
                            }

                            if (m_text == tr("收取已发送邮件"))
                            {
                                emit toggled(m_bChecked, email::SwitchControl_Enum::ReceiveSent);
                            }

                            if (m_text == tr("收取草稿箱邮件"))
                            {
                                emit toggled(m_bChecked, email::SwitchControl_Enum::ReceiveDraft);
                            }

                            m_timer.start(2);
                        }
                        else {
                            event->ignore();
                        }
                    }
                }


                void SwitchControl::resizeEvent(QResizeEvent* event)
                {
                    m_nX = m_nHeight / 2;
                    m_nY = m_nHeight / 2;
                    QWidget::resizeEvent(event);
                }


                QSize SwitchControl::sizeHint() const
                {
                    return minimumSizeHint();
                }


                QSize SwitchControl::minimumSizeHint() const
                {
                    return QSize(2 * (m_nHeight + m_nMargin), m_nHeight + 2 * m_nMargin);
                }

                void SwitchControl::onTimeout()
                {
                    if (m_bChecked) {
                        m_nX += 1;
                        if (m_nX >= width() - m_nHeight -1)
                            m_timer.stop();
                    }
                    else {
                        m_nX -= 1;
                        if (m_nX <= m_nHeight / 2)
                            m_timer.stop();
                    }
                    update();
                }
                
                bool SwitchControl::isToggled() const
                {
                    return m_bChecked;
                }

                void SwitchControl::setToggle(bool checked)
                {
                    m_bChecked = checked;
                    m_timer.start(1);
                }
                
                void SwitchControl::setBackgroundColor(QColor color)
                {
                    m_background = color;
                }
                
                void SwitchControl::setCheckedColor(QColor color)
                {
                    m_checkbackground = color;
                }

                void SwitchControl::setDisbaledColor(QColor color)
                {
                    m_disabledColor = color;
                }

                int SwitchControl::as_96dpi(int pixels_in_96dpi) const {
                    return gui::as_96dpi(this, pixels_in_96dpi);
                }


            }
        }
    }
}
