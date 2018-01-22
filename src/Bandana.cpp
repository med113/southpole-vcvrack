#include "Southpole.hpp"
#include <string.h>


struct Bandana : Module {
	enum ParamIds {
		GAIN1_PARAM,
		GAIN2_PARAM,
		GAIN3_PARAM,
		GAIN4_PARAM,
		MOD1_PARAM,
		MOD2_PARAM,
		MOD3_PARAM,
		MOD4_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		IN3_INPUT,
		IN4_INPUT,
		CV1_INPUT,
		CV2_INPUT,
		CV3_INPUT,
		CV4_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		OUT3_OUTPUT,
		OUT4_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		CV1_POS_LIGHT, CV1_NEG_LIGHT,
		CV2_POS_LIGHT, CV2_NEG_LIGHT,
		CV3_POS_LIGHT, CV3_NEG_LIGHT,
		CV4_POS_LIGHT, CV4_NEG_LIGHT,
		OUT1_POS_LIGHT, OUT1_NEG_LIGHT,
		OUT2_POS_LIGHT, OUT2_NEG_LIGHT,
		OUT3_POS_LIGHT, OUT3_NEG_LIGHT,
		OUT4_POS_LIGHT, OUT4_NEG_LIGHT,
		NUM_LIGHTS
	};

	Bandana() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	void step() override;
};


void Bandana::step() {
	float out = 0.0;

	for (int i = 0; i < 4; i++) {
		float g = params[GAIN1_PARAM + i].value;
		g += params[MOD1_PARAM + i].value * inputs[CV1_INPUT + i].value / 5.0;
		g = clampf(g, -2.0, 2.0);
		lights[CV1_POS_LIGHT + 2*i].setBrightnessSmooth(fmaxf(0.0, g));
		lights[CV1_NEG_LIGHT + 2*i].setBrightnessSmooth(fmaxf(0.0, -g));
		out += g * inputs[IN1_INPUT + i].normalize(5.0);
		lights[OUT1_POS_LIGHT + 2*i].setBrightnessSmooth(fmaxf(0.0, out / 5.0));
		lights[OUT1_NEG_LIGHT + 2*i].setBrightnessSmooth(fmaxf(0.0, -out / 5.0));
		if (outputs[OUT1_OUTPUT + i].active) {
			outputs[OUT1_OUTPUT + i].value = out;
			out = 0.0;
		}
	}
}


BandanaWidget::BandanaWidget() {
	Bandana *module = new Bandana();
	setModule(module);
	box.size = Vec(15*6, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/Bandana.svg")));
		panel->box.size = box.size;
		addChild(panel);	
	}

    const float x1 = 8.;
    const float x2 = 34.;
    const float x3 = 62.;	

	addParam(createParam<sp_SmallBlackKnob>(Vec(x2, 52+8), module, Bandana::GAIN1_PARAM, -1.0, 1.0, 0.0));
	addParam(createParam<sp_SmallBlackKnob>(Vec(x2, 131+8), module, Bandana::GAIN2_PARAM, -1.0, 1.0, 0.0));
	addParam(createParam<sp_SmallBlackKnob>(Vec(x2, 210+8), module, Bandana::GAIN3_PARAM, -1.0, 1.0, 0.0));
	addParam(createParam<sp_SmallBlackKnob>(Vec(x2, 288+8), module, Bandana::GAIN4_PARAM, -1.0, 1.0, 0.0));

	addParam(createParam<sp_Trimpot>(Vec(x3-10,  80+8), module, Bandana::MOD1_PARAM, -1.0, 1.0, 0.0));
	addParam(createParam<sp_Trimpot>(Vec(x3-10, 159+8), module, Bandana::MOD2_PARAM, -1.0, 1.0, 0.0));
	addParam(createParam<sp_Trimpot>(Vec(x3-10, 238+8), module, Bandana::MOD3_PARAM, -1.0, 1.0, 0.0));
	addParam(createParam<sp_Trimpot>(Vec(x3-10, 316+8), module, Bandana::MOD4_PARAM, -1.0, 1.0, 0.0));

	addInput(createInput<PJ301MPort>(Vec(x1, 41), module, Bandana::IN1_INPUT));
	addInput(createInput<PJ301MPort>(Vec(x1, 120), module, Bandana::IN2_INPUT));
	addInput(createInput<PJ301MPort>(Vec(x1, 198), module, Bandana::IN3_INPUT));
	addInput(createInput<PJ301MPort>(Vec(x1, 277), module, Bandana::IN4_INPUT));

	addInput(createInput<PJ301MPort>(Vec(x1, 80), module, Bandana::CV1_INPUT));
	addInput(createInput<PJ301MPort>(Vec(x1, 159), module, Bandana::CV2_INPUT));
	addInput(createInput<PJ301MPort>(Vec(x1, 238), module, Bandana::CV3_INPUT));
	addInput(createInput<PJ301MPort>(Vec(x1, 316), module, Bandana::CV4_INPUT));

	addOutput(createOutput<PJ301MPort>(Vec(x3, 41), module, Bandana::OUT1_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(x3, 120), module, Bandana::OUT2_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(x3, 198), module, Bandana::OUT3_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(x3, 277), module, Bandana::OUT4_OUTPUT));

	addChild(createLight<SmallLight<GreenRedLight>>(Vec(x2+9,  96-46), module, Bandana::CV1_POS_LIGHT));
	addChild(createLight<SmallLight<GreenRedLight>>(Vec(x2+9, 175-46), module, Bandana::CV2_POS_LIGHT));
	addChild(createLight<SmallLight<GreenRedLight>>(Vec(x2+9, 254-46), module, Bandana::CV3_POS_LIGHT));
	addChild(createLight<SmallLight<GreenRedLight>>(Vec(x2+9, 333-46), module, Bandana::CV4_POS_LIGHT));

	addChild(createLight<MediumLight<GreenRedLight>>(Vec(x3+8, 87-6), module, Bandana::OUT1_POS_LIGHT));
	addChild(createLight<MediumLight<GreenRedLight>>(Vec(x3+8, 166-6), module, Bandana::OUT2_POS_LIGHT));
	addChild(createLight<MediumLight<GreenRedLight>>(Vec(x3+8, 245-6), module, Bandana::OUT3_POS_LIGHT));
	addChild(createLight<MediumLight<GreenRedLight>>(Vec(x3+8, 324-6), module, Bandana::OUT4_POS_LIGHT));
}
