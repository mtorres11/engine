--local value=Vect3f(50, 10, 0)
--get_core():get_light_manager():get_resource("Spot01"):set_position(value)

local l_position=Vect3f(-25+math.random()*50,0.0, -25+math.random()*50)
local new_id=get_new_id()
get_core():get_renderable_objects_manager():add_mesh_instance("tumba","my_instance_"..new_id, l_position, 0.0, 0.0, 0.0)