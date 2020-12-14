#pragma once
#ifndef COREMAIL_CLIENT_APP_GUI_EMAIL_SWITCH_CONTROL_HPP_
#define COREMAIL_CLIENT_APP_GUI_EMAIL_SWITCH_CONTROL_HPP_
#include "worker.hpp"
#include <QWidget>
#include <QTimer>

namespace coremail {
    namespace client_app {
        namespace gui {
            namespace email {

                enum class SwitchControl_Enum {
                    ReceiveSpam = 0,
                    ReceiveSent = 1,
                    ReceiveDraft = 2
                };

                Q_DECLARE_METATYPE(SwitchControl_Enum)

                class SwitchControl : public QWidget
                {
                    Q_OBJECT

                public:
                    explicit SwitchControl(const QString& atext, const BOOL& b, QWidget* parent = 0);

                    bool isToggled() const;
                    void setToggle(bool checked);
                    void setBackgroundColor(QColor color);
                    void setCheckedColor(QColor color);
                    void setDisbaledColor(QColor color);

                protected:
                    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
                    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
                    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
                    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

                    QSize sizeHint() const Q_DECL_OVERRIDE;
                    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

                signals:
                    void toggled(bool checked, SwitchControl_Enum ctrl);
                    void button_clicked(SwitchControl_Enum ctrl);

                private slots:
                    void onTimeout();

                private:
                    bool m_bChecked;         
                    QColor m_checkbackground;
                    QColor m_uncheckbackground;
                    QColor m_background;     
                    QColor m_disabledColor;  
                    QColor m_thumbColor;     
                    qreal m_radius;          
                    qreal m_nX;              
                    qreal m_nY;              
                    qint16 m_nHeight;        
                    qint16 m_nMargin;        
                    QTimer m_timer;          
                    static std::once_flag	type_register_flag;
                    QString m_text = tr("");
                    BOOL m_bCheckStatus = false;

                private:
                    int as_96dpi(int pixels_in_96dpi) const;


                };
            }
        }
    }
}
#endif
