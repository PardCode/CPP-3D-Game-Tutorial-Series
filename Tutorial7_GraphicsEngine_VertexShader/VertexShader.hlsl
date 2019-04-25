


float4 vsmain( float4 position: POSITION): SV_POSITION
{
	if (position.y > 0 && position.y < 1)
	{
		position.x += 0.25f;
	}


	if (position.y > 0 && position.y < 1 && position.x > -1 && position.x < 0)
	{
		position.y -= 0.25f;
	}


	return position;
}