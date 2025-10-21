#include "raylib.h"
#include "UI.h"


Button::Button() = default;

void Button::SetState(State newState)
{
    if (m_state != newState)
    {
        m_state = newState;
    }
}

Button::State Button::GetState() const
{
    return m_state;
}

UI::UI() = default;

void UI::Update( Audio& audio)
{
	m_mousePos = GetMousePosition();
    m_mouseButtonDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
	m_mouseButtonReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    //reset active id
    m_activeButtonID = -1;

    for (int i = 0; i < m_buttons.size(); i++)
    {
        Button& button { m_buttons[i] };
        Button::State nextState { Button::State::DEFAULT };

        // Check if mouse is over button
        if (CheckCollisionPointRec(m_mousePos, button.bounds))
        {
            nextState = Button::State::HOVERED;
            m_activeButtonID = button.id;

            if (m_mouseButtonDown)
            {

                nextState = Button::State::PRESSED;
            }

            if (m_mouseButtonReleased)
            {
                audio.PlaySoundRandomisedPitch(Audio::SFXID::MENU_BUTTON);
                nextState = Button::State::RELEASED;
            }
        }

        button.SetState(nextState);
    }
}

void UI::AddButton(int id, Rectangle bounds, const char* text)
{
    Button newButton {};
    newButton.bounds = bounds;
    newButton.id = id;
    newButton.text = text;
    newButton.SetState(Button::State::DEFAULT);

    m_buttons.push_back(newButton);
}

std::vector<Button> UI::GetButtons() const
{
    return m_buttons;
}

// Checks if a button is released while active
// To be active and released the button needs to 
// be hovered over (active button) and then released 
bool UI::WasActiveButtonReleased(int buttonID)
{
    if (m_activeButtonID != buttonID)
    {
        return false;
    }

    if (buttonID < 0 && buttonID > m_buttons.size())
    {
        return false;
    }

    Button& button { m_buttons[buttonID] };
    Button::State buttonState { button.GetState() };

    return buttonState == Button::State::RELEASED;
}

void UI::CentreButtonsVertically(int screenWidth, int screenHeight, int buttonWidth, int buttonHeight, int padding)
{
    if (m_buttons.size() == 0)
    {
        return;
    }
    // button height + padding
    const int totalButtonsHeight { static_cast<int>((m_buttons.size() * buttonHeight) +
        ((m_buttons.size() - 1) * padding)) };
    const float startY { static_cast<float>((screenHeight / 2) - (totalButtonsHeight / 2)) };

    // update new button positions
    for (int i = 0; i < m_buttons.size(); i++)
    {
        m_buttons[i].bounds = {
            (screenWidth / 2.0f) - (buttonWidth / 2.0f),
            startY + (i * (buttonHeight + padding)),
            static_cast<float>(buttonWidth),
            static_cast<float>(buttonHeight)
        };
    }
}





