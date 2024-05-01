/*
  =============================================================================

    VerticalGradientMete.h
    
    Main Author: Created by Akash Murthy https://github.com/Thrifleganger
    Author (customized to my project): Michael Kauffmann

	A Meter(VerticalGradientMeter) represent 1 ch. So two instances are needed to form a 2 ch Meter. A Meter is a cild component MFXGainPanel(client), where Bounds are set.
	

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
			DBG("value" << valueSupplier());
			g.setGradientFill(gradient);
			const auto scaledY = jmap(valueSupplier(), -60.0f, 6.0f, 0.0f, static_cast<float>(getHeight())); 
            
            // Prevent drawing if no audio input goes into plugin
            if (scaledY > 0)
            g.fillRect(bounds.removeFromBottom(scaledY));  // remove part of bounds, and will paint the returned rectangle at removed area, bounds determent how much of the gradient is visible.
		}

		void resized() override
		{
			// Why not put this in constructor. Because bounds will be all 0, not known in constructor.

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
