#include "proto.hpp"
namespace proto {
	bool has_bits::test( std::uint32_t mask ) const { return ( this->bits[ 0 ] & mask ) != 0; }
	void has_bits::set( std::uint32_t mask ) { this->bits[ 0 ] |= mask; }
	void has_bits::clear( std::uint32_t mask ) { this->bits[ 0 ] &= ~mask; }
	float msg_qangle::x( ) const { return this->m_x; }
	float msg_qangle::y( ) const { return this->m_y; }
	float msg_qangle::z( ) const { return this->m_z; }
	void msg_qangle::set_x( float v ) { this->m_has_bits.set( 0x1u ); this->m_x = v; }
	void msg_qangle::set_y( float v ) { this->m_has_bits.set( 0x2u ); this->m_y = v; }
	void msg_qangle::set_z( float v ) { this->m_has_bits.set( 0x4u ); this->m_z = v; }
	void interpolation_info::set_frac( float v ) { this->m_has_bits.set( 0x1u ); this->m_frac = v; }
	void interpolation_info::set_src_tick( std::int32_t v ) { this->m_has_bits.set( 0x2u ); this->m_src_tick = v; }
	void interpolation_info::set_dst_tick( std::int32_t v ) { this->m_has_bits.set( 0x4u ); this->m_dst_tick = v; }
	void interpolation_info_cl::set_frac( float v ) { this->m_has_bits.set( 0x1u ); this->m_frac = v; }
	std::uint64_t in_button_state_pb::buttonstate1( ) const { return this->m_buttonstate1; }
	std::uint64_t in_button_state_pb::buttonstate2( ) const { return this->m_buttonstate2; }
	std::uint64_t in_button_state_pb::buttonstate3( ) const { return this->m_buttonstate3; }
	void in_button_state_pb::set_buttonstate1( std::uint64_t v ) { this->m_has_bits.set( 0x1u ); this->m_buttonstate1 = v; }
	void in_button_state_pb::set_buttonstate2( std::uint64_t v ) { this->m_has_bits.set( 0x2u ); this->m_buttonstate2 = v; }
	void in_button_state_pb::set_buttonstate3( std::uint64_t v ) { this->m_has_bits.set( 0x4u ); this->m_buttonstate3 = v; }
	std::uint64_t subtick_move_step::button( ) const { return this->m_button; }
	void subtick_move_step::set_button( std::uint64_t v ) { this->m_has_bits.set( 0x1u ); this->m_button = v; }
	bool subtick_move_step::pressed( ) const { return this->m_pressed; }
	void subtick_move_step::set_pressed( bool v ) { this->m_has_bits.set( 0x2u ); this->m_pressed = v; }
	float subtick_move_step::when( ) const { return this->m_when; }
	void subtick_move_step::set_when( float v ) { this->m_has_bits.set( 0x4u ); this->m_when = v; }
	void subtick_move_step::set_analog_forward_delta( float v ) { this->m_has_bits.set( 0x8u ); this->m_analog_forward_delta = v; }
	void subtick_move_step::set_analog_left_delta( float v ) { this->m_has_bits.set( 0x10u ); this->m_analog_left_delta = v; }
	void subtick_move_step::set_pitch_delta( float v ) { this->m_has_bits.set( 0x20u ); this->m_pitch_delta = v; }
	void subtick_move_step::set_yaw_delta( float v ) { this->m_has_bits.set( 0x40u ); this->m_yaw_delta = v; }
	const in_button_state_pb* base_usercmd_pb::buttons_pb( ) const { return impl_ptr<const in_button_state_pb>( this->m_buttons_pb ); }
	const msg_qangle* base_usercmd_pb::viewangles( ) const { return impl_ptr<const msg_qangle>( this->m_viewangles ); }
	msg_qangle* base_usercmd_pb::mutable_viewangles( ) { this->m_has_bits.set( 0x4u ); return impl_ptr<msg_qangle>( this->m_viewangles ); }
	repeated_ptr_field<subtick_move_step>* base_usercmd_pb::mutable_subtick_moves( ) { return &this->m_subtick_moves; }
	int base_usercmd_pb::subtick_moves_size( ) const { return this->m_subtick_moves.size( ); }
	subtick_move_step* base_usercmd_pb::mutable_subtick_moves( int i ) { return this->m_subtick_moves.mutable_at( i ); }
	void base_usercmd_pb::set_forwardmove( float v ) { this->m_has_bits.set( 0x40u ); this->m_forwardmove = v; }
	void base_usercmd_pb::set_leftmove( float v ) { this->m_has_bits.set( 0x80u ); this->m_leftmove = v; }
	void base_usercmd_pb::set_weaponselect( std::int32_t v ) { this->m_has_bits.set( 0x400u ); this->m_weaponselect = v; }
	std::int32_t base_usercmd_pb::client_tick( ) const { return this->m_client_tick; }
	float base_usercmd_pb::forwardmove( ) const { return this->m_forwardmove; }
	float base_usercmd_pb::leftmove( ) const { return this->m_leftmove; }
	msg_qangle* input_history_entry::mutable_view_angles( ) { this->m_has_bits.set( 0x1u ); return impl_ptr<msg_qangle>( this->m_view_angles ); }
	bool input_history_entry::has_cl_interp( ) const { return this->m_has_bits.test( 0x2u ); }
	interpolation_info_cl* input_history_entry::mutable_cl_interp( ) { this->m_has_bits.set( 0x2u ); return impl_ptr<interpolation_info_cl>( this->m_cl_interp ); }
	bool input_history_entry::has_sv_interp0( ) const { return this->m_has_bits.test( 0x4u ); }
	interpolation_info* input_history_entry::mutable_sv_interp0( ) { this->m_has_bits.set( 0x4u ); return impl_ptr<interpolation_info>( this->m_sv_interp0 ); }
	bool input_history_entry::has_sv_interp1( ) const { return this->m_has_bits.test( 0x8u ); }
	interpolation_info* input_history_entry::mutable_sv_interp1( ) { this->m_has_bits.set( 0x8u ); return impl_ptr<interpolation_info>( this->m_sv_interp1 ); }
	void input_history_entry::set_render_tick_count( std::int32_t v ) { this->m_has_bits.set( 0x200u ); this->m_render_tick_count = v; }
	void input_history_entry::set_render_tick_fraction( float v ) { this->m_has_bits.set( 0x400u ); this->m_render_tick_fraction = v; }
	void input_history_entry::set_player_tick_count( std::int32_t v ) { this->m_has_bits.set( 0x800u ); this->m_player_tick_count = v; }
	void input_history_entry::set_player_tick_fraction( float v ) { this->m_has_bits.set( 0x1000u ); this->m_player_tick_fraction = v; }
	bool csgo_usercmd_pb::has_base( ) const { return this->m_has_bits.test( 0x1u ); }
	base_usercmd_pb* csgo_usercmd_pb::mutable_base( ) { this->m_has_bits.set( 0x1u ); return impl_ptr<base_usercmd_pb>( this->m_base ); }
	const base_usercmd_pb* csgo_usercmd_pb::base( ) const { return impl_ptr<const base_usercmd_pb>( this->m_base ); }
	int csgo_usercmd_pb::input_history_size( ) const { return this->m_input_history.size( ); }
	input_history_entry* csgo_usercmd_pb::mutable_input_history( int i ) { return this->m_input_history.mutable_at( i ); }
	void csgo_usercmd_pb::set_attack1_start_history_index( std::int32_t v ) { this->m_has_bits.set( 0x20u ); this->m_attack1_start_history_index = v; }
	void csgo_usercmd_pb::set_attack2_start_history_index( std::int32_t v ) { this->m_has_bits.set( 0x40u ); this->m_attack2_start_history_index = v; }
	std::int32_t csgo_usercmd_pb::attack1_start_history_index( ) const { return this->m_attack1_start_history_index; }
	std::int32_t csgo_usercmd_pb::attack2_start_history_index( ) const { return this->m_attack2_start_history_index; }
}
