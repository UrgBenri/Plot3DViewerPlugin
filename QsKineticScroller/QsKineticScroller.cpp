/*
	This file is part of the UrgBenri application.

	Copyright (c) 2016 Mehrez Kristou.
	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Please contact kristou@hokuyo-aut.jp for more details.

*/

// Copyright (c) 2011, Razvan Petru
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or other
//   materials provided with the distribution.
// * The name of the contributors may not be used to endorse or promote products
//   derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#include "QsKineticScroller.h"
#include <QApplication>
#include <QScrollBar>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QAbstractScrollArea>
#include <QMouseEvent>
#include <QEvent>
#include <QTimer>
#include <QList>
#include <QtDebug>
#include <cstddef> // for NULL

// A number of mouse moves are ignored after a press to differentiate
// it from a press & drag.
static const int gMaxIgnoredMouseMoves = 10;
// The timer measures the drag speed & handles kinetic scrolling. Adjusting
// the timer interval will change the scrolling speed and smoothness.
static const int gTimerInterval = 30;
// The speed measurement is imprecise, limit it so that the scrolling is not
// too fast.
static const int gMaxDecelerationSpeed = 30;
// influences how fast the scroller decelerates
static const int gFriction = 1;

//#define QS_KINETIC_DEBUG // uncomment to see debug messages
#ifdef QS_KINETIC_DEBUG
#define SCROLL_DEBUG() qDebug()
#else
#define SCROLL_DEBUG() QNoDebug()
#endif

class QsKineticScrollerImpl
{
public:
    QsKineticScrollerImpl()
        : scrollArea(NULL)
        , isPressed(false)
        , isMoving(false)
        , lastMouseYPos(0)
        , lastScrollBarPosition(0)
        , velocity(0)
        , ignoredMouseMoves(0) {}

    void stopMotion()
    {
        isMoving = false;
        velocity = 0;
        kineticTimer.stop();
    }

    QAbstractScrollArea* scrollArea;
    bool isPressed;
    bool isMoving;
    QPoint lastPressPoint;
    int lastMouseYPos;
    int lastScrollBarPosition;
    int velocity;
    int ignoredMouseMoves;
    QTimer kineticTimer;

    QList<QEvent*> ignoreList;
};

QsKineticScroller::QsKineticScroller(QObject *parent)
    : QObject(parent)
    , d(new QsKineticScrollerImpl)
{
    connect(&d->kineticTimer, &QTimer::timeout, this, &QsKineticScroller::onKineticTimerElapsed);
}

// needed by smart pointer
QsKineticScroller::~QsKineticScroller()
{
}

void QsKineticScroller::enableKineticScrollFor(QAbstractScrollArea* scrollArea)
{
    if( !scrollArea )
    {
        Q_ASSERT_X(0, "kinetic scroller", "missing scroll area");
        return;
    }

    // remove existing association
    if( d->scrollArea )
    {
        QList<QLineEdit*> edits = d->scrollArea->findChildren<QLineEdit*>();
        Q_FOREACH(QLineEdit *edit, edits)
            edit->removeEventFilter(this);
        QList<QTextEdit*> textEdits = d->scrollArea->findChildren<QTextEdit*>();
        Q_FOREACH(QTextEdit *textEdit, textEdits)
            textEdit->viewport()->removeEventFilter(this);
        QList<QComboBox*> combos = d->scrollArea->findChildren<QComboBox*>();
        Q_FOREACH(QComboBox *combo, combos)
            combo->removeEventFilter(this);
//        QList<QGroupBox*> groups = d->scrollArea->findChildren<QGroupBox*>();
//        Q_FOREACH(QGroupBox *group, groups)
//            group->removeEventFilter(this);
        QList<QSpinBox*> spins = d->scrollArea->findChildren<QSpinBox*>();
        Q_FOREACH(QSpinBox *spin, spins)
            spin->removeEventFilter(this);
        QList<QCheckBox*> checks = d->scrollArea->findChildren<QCheckBox*>();
        Q_FOREACH(QCheckBox *check, checks)
            check->removeEventFilter(this);
        QList<QRadioButton*> radios = d->scrollArea->findChildren<QRadioButton*>();
        Q_FOREACH(QRadioButton *radio, radios)
            radio->removeEventFilter(this);
        QList<QPushButton*> pushes = d->scrollArea->findChildren<QPushButton*>();
        Q_FOREACH(QPushButton *push, pushes)
            push->removeEventFilter(this);
        QList<QLabel*> labels = d->scrollArea->findChildren<QLabel*>();
        Q_FOREACH(QLabel *label, labels)
            label->removeEventFilter(this);
        d->scrollArea->viewport()->removeEventFilter(this);
        d->scrollArea->removeEventFilter(this);
        d->scrollArea = NULL;
    }

    // associate
    scrollArea->installEventFilter(this);
    scrollArea->viewport()->installEventFilter(this);
    d->scrollArea = scrollArea;

    QList<QLineEdit*> edits = scrollArea->findChildren<QLineEdit*>();
    Q_FOREACH(QLineEdit *edit, edits)
        edit->installEventFilter(this);
    QList<QTextEdit*> textEdits = scrollArea->findChildren<QTextEdit*>();
    Q_FOREACH(QTextEdit *textEdit, textEdits)
        textEdit->viewport()->installEventFilter(this);
    QList<QComboBox*> combos = scrollArea->viewport()->findChildren<QComboBox*>();
    Q_FOREACH(QComboBox *combo, combos)
        combo->installEventFilter(this);
//    QList<QGroupBox*> groupBoxes = scrollArea->viewport()->findChildren<QGroupBox*>();
//    Q_FOREACH(QGroupBox *group, groupBoxes)
//        group->installEventFilter(this);
    QList<QSpinBox*> spinBoxes = scrollArea->viewport()->findChildren<QSpinBox*>();
    Q_FOREACH(QSpinBox *spin, spinBoxes)
        spin->installEventFilter(this);
    QList<QCheckBox*> checkBoxes = scrollArea->viewport()->findChildren<QCheckBox*>();
    Q_FOREACH(QCheckBox *check, checkBoxes)
        check->installEventFilter(this);
    QList<QRadioButton*> radioButtons = scrollArea->viewport()->findChildren<QRadioButton*>();
    Q_FOREACH(QRadioButton *radio, radioButtons)
        radio->installEventFilter(this);
    QList<QPushButton*> pushButtons = scrollArea->viewport()->findChildren<QPushButton*>();
    Q_FOREACH(QPushButton *push, pushButtons)
        push->installEventFilter(this);
    QList<QLabel*> labels = scrollArea->viewport()->findChildren<QLabel*>();
    Q_FOREACH(QLabel *label, labels)
        label->installEventFilter(this);
}

//! intercepts mouse events to make the scrolling work
bool QsKineticScroller::eventFilter(QObject* object, QEvent* event)
{
    const QEvent::Type eventType = event->type();
    const bool isMouseAction = QEvent::MouseButtonPress == eventType
            || QEvent::MouseButtonRelease == eventType;
    const bool isMouseEvent = isMouseAction || QEvent::MouseMove == eventType;

    if( !isMouseEvent || !d->scrollArea )
        return false;

    if( isMouseAction && d->ignoreList.removeAll(event) > 0 ) // This event is ignored
        return false;

    QMouseEvent* const mouseEvent = static_cast<QMouseEvent*>(event);
    QTouchEvent* const touchEvent = static_cast<QTouchEvent*>(event);
    switch( eventType )
    {
    case QEvent::TouchBegin:
    case QEvent::MouseButtonPress:
    {
        QPoint pressPos;
        if(mouseEvent){
            pressPos = mouseEvent->globalPos();
        }else if(touchEvent){
            pressPos = touchEvent->touchPoints()[0].screenPos().toPoint();
        }
        SCROLL_DEBUG() << "[scroll] press pos" << pressPos;
        d->lastPressPoint = pressPos;
        d->isPressed = true;
        d->lastScrollBarPosition = d->scrollArea->verticalScrollBar()->value();
        if( d->isMoving ) // press while kinetic scrolling, so stop
            d->stopMotion();
    }
        break;
    case QEvent::TouchUpdate:
    case QEvent::MouseMove:
    {
        QPoint movePos;
        if(mouseEvent){
            movePos = mouseEvent->globalPos();
        }else if(touchEvent){
            movePos = touchEvent->touchPoints()[0].screenPos().toPoint();
        }
        SCROLL_DEBUG() << "[scroll] move pos" << movePos;
        if( !d->isMoving )
        {
            // A few move events are ignored as "click jitter", but after that we
            // assume that the user is doing a click & drag
            if( d->ignoredMouseMoves < gMaxIgnoredMouseMoves )
                ++d->ignoredMouseMoves;
            else
            {
                d->lastMouseYPos = movePos.y();
                d->ignoredMouseMoves = 0;
                d->isMoving = true;
                if( !d->kineticTimer.isActive() )
                    d->kineticTimer.start(gTimerInterval);
            }
        }
        else
        {
            // manual scroll
            const int dragDistance = movePos.y() - d->lastPressPoint.y();
            const int finalScrollPos = d->lastScrollBarPosition - dragDistance;
            d->scrollArea->verticalScrollBar()->setValue(finalScrollPos);
            SCROLL_DEBUG() << "[scroll] manual scroll to" << finalScrollPos;
        }
    }
        break;
    case QEvent::HoverLeave:
    case QEvent::TouchEnd:
    case QEvent::MouseButtonRelease:
    {
        QPoint releasePos;
        if(mouseEvent){
            releasePos = mouseEvent->pos();
        }else if(touchEvent){
            releasePos = touchEvent->touchPoints()[0].scenePos().toPoint();
        }
        SCROLL_DEBUG() << "[scroll] release pos" << releasePos;
        d->isPressed = false;
        d->ignoredMouseMoves = 0;
        // Looks like the user wanted a single click. Simulate the click,
        // as the events were already consumed
        const bool isEditWidget = (
                    NULL != qobject_cast<QLineEdit*>(object)
                || NULL != qobject_cast<QTextEdit*>(object->parent())
                || NULL != qobject_cast<QSpinBox*>(object)
                || NULL != qobject_cast<QCheckBox*>(object)
                || NULL != qobject_cast<QRadioButton*>(object)
                || NULL != qobject_cast<QPushButton*>(object)
                || NULL != qobject_cast<QComboBox*>(object)
                );
        const bool isInterestingObject = (object == d->scrollArea->viewport() || isEditWidget);
        if( !d->isMoving && isInterestingObject )
        {
            if(mouseEvent){
                QMouseEvent* mousePress = new QMouseEvent(QEvent::MouseButtonPress,
                                                          releasePos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
                QMouseEvent* mouseRelease = new QMouseEvent(QEvent::MouseButtonRelease,
                                                            releasePos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

                // Ignore these two
                d->ignoreList << mousePress;
                d->ignoreList << mouseRelease;

                QApplication::postEvent(object, mousePress);
                QApplication::postEvent(object, mouseRelease);
            }else if(touchEvent){
                QTouchEvent* touchBegin = new QTouchEvent(QEvent::TouchBegin, touchEvent->device(), Qt::NoModifier,
                                                          touchEvent->touchPointStates(), touchEvent->touchPoints());
                QTouchEvent* touchEnd = new QTouchEvent(QEvent::TouchEnd, touchEvent->device(), Qt::NoModifier,
                                                        touchEvent->touchPointStates(), touchEvent->touchPoints());

                // Ignore these two
                d->ignoreList << touchBegin;
                d->ignoreList << touchEnd;

                QApplication::postEvent(object, touchBegin);
                QApplication::postEvent(object, touchEnd);
            }
            if (isEditWidget) {
                SCROLL_DEBUG() << "[scroll] trying to open keyboard";
                QEvent *openKeyboard = new QEvent(QEvent::RequestSoftwareInputPanel);
                QApplication::postEvent(object, openKeyboard);
            }
        }
    }
        break;
    default:
        break;
    }

    return true; // filter event
}

void QsKineticScroller::onKineticTimerElapsed()
{
    if( d->isPressed && d->isMoving )
    {
        // the speed is measured between two timer ticks
        const QPoint pos = QCursor::pos();
        const int cursorYPos = pos.y();
        d->velocity = cursorYPos - d->lastMouseYPos;
        d->lastMouseYPos = cursorYPos;
        SCROLL_DEBUG() << "[scroll] cursor Y pos" << cursorYPos << d->lastMouseYPos;
    }
    else if( !d->isPressed && d->isMoving )
    {
        // use the previously recorded speed and gradually decelerate
        d->velocity = qBound(-gMaxDecelerationSpeed, d->velocity, gMaxDecelerationSpeed);
        if( d->velocity > 0 )
            d->velocity -= gFriction;
        else if( d->velocity < 0 )
            d->velocity += gFriction;
        if( qAbs(d->velocity) < qAbs(gFriction) )
            d->stopMotion();

        const int scrollBarYPos = d->scrollArea->verticalScrollBar()->value();
        const int finalScrollPos = scrollBarYPos - d->velocity;
        SCROLL_DEBUG() << "[scroll] kinetic scroll to" << finalScrollPos << "with velocity" << d->velocity;
        d->scrollArea->verticalScrollBar()->setValue(finalScrollPos);
    }
    else{
        d->stopMotion();
    }
}

