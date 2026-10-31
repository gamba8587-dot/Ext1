#pragma once
namespace random {
	float floating( float min, float max );
	float normal( float mean, float stddev );
	float normal_clamped( float mean, float stddev, float min, float max );
	float hold_duration( float mean = 0.085f, float stddev = 0.012f );
}
