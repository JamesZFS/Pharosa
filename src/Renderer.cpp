//
// Created by think on 2019/4/1.
//

#include "Renderer.h"

template<typename GI_Algorithm, typename Cameras_Type>
Renderer<GI_Algorithm, Cameras_Type>::Renderer()
{
	stage = new Stage();
}

template<typename GI_Algorithm, typename Cameras_Type>
Renderer<GI_Algorithm, Cameras_Type>::~Renderer()
{

}
