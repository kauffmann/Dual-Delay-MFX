/*
  =============================================================================

    VerticalGradientMete.h
    
    Main Author: Created by Akash Murthy https://github.com/Thrifleganger
    Author (customized to my project): Michael Kauffmann

 = =============================================================================
*/
#pragma once

#include <JuceHeader.h>


	class VerticalGradientMeter : public Component, public Timer
	{
	public:
		VerticalGradientMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction)) 
		{
			startTimerHz(24);
			grill = ImageCache::getFromMemory(BinaryData::MeterBG2_png, BinaryData::MeterBG2_pngSize);
		}
		
		void paint(Graphics& g) override
		{
			auto bounds = getLocalBounds().toFloat().reduced(3.5f);

            g.setColour(colour_2);
			g.fillRect(bounds);

			g.setGradientFill(gradient);
			const auto scaledY = jmap(valueSupplier(), -60.0f, 6.0f, 0.0f, static_cast<float>(getHeight())); 
            
            // Prevent drawing if no audio input goes into plugin
            if (scaledY > 0)
            g.fillRect(bounds.removeFromBottom(scaledY));
		}

		void resized() override
		{
			const auto bounds = getLocalBounds().toFloat();
            gradient = ColourGradient{ Colours::white, bounds.getBottomLeft(), Colours::red, bounds.getTopLeft(), false };
            gradient.addColour(0.7, Colours::white);
            
		}
		
		void paintOverChildren(::juce::Graphics& g) override
		{
			g.drawImage(grill, getLocalBounds().toFloat().reduced(3.0f));
		}

		void timerCallback() override
		{
			repaint();
		}
	private:
		std::function<float()> valueSupplier;
		ColourGradient gradient{};
		Image grill;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VerticalGradientMeter);

    };
