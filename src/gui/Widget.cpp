#include "Widget.hpp"
#include "Menu.hpp"

using namespace gui;

Widget::Widget(Menu* owner, bool focusable):
    owner_(owner),
    width_(0),
    height_(0),
    focusable_(focusable),
    visible_(true),
    state_(State::DEFAULT)
{
    owner->AddWidget(this);
}


void Widget::SetState(State::EState state)
{
    OnStateChanged(state);
    state_ = state;
}


State::EState Widget::GetState() const
{
    return state_;
}


void Widget::Update(float)
{
}


bool Widget::ContainsPoint(float x, float y)
{
    return (x >= getPosition().x) && (x <= getPosition().x + width_) && (y >= getPosition().y) && (y <= getPosition().y + height_);
}


void Widget::Resize(int width, int height)
{
    width_ = width;
    height_ = height;
}


int Widget::GetWidth() const
{
    return width_;
}


int Widget::GetHeight() const
{
    return height_;
}


bool Widget::CanGrabFocus() const
{
    return focusable_ && visible_ && state_ != State::DISABLED;
}


bool Widget::IsFocused() const
{
    return this == GetOwner()->GetFocusedWidget();
}


Menu* Widget::GetOwner() const
{
    return owner_;
}


void Widget::setCallback(std::function<void(void)> callback)
{
    callback_ = callback;
}


void Widget::triggerCallback()
{
    if (callback_)
    {
        callback_();
    }
}


void Widget::centerText(sf::Text& text)
{
    float textHeight = text.getFont()->getLineSpacing(text.getCharacterSize());
    float textWidth = text.getLocalBounds().width;
    // Cast position to int for pixel-perfect rendering
    text.setPosition(
        (int) (width_ - textWidth) / 2,
        (int) (height_ - textHeight) / 2
    );
}
