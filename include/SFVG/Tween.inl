// Evan Pezent (epezent@rice.edu, evanpezent.com)
// 3/14/2017

using UnityEngine;

public class Tween
{

    public static float Instant(float a, float b, float t)
    {
        return b;
    }

    public static float Linear(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    public static float Smoothstep(float a, float b, float t)
    {
        t = t * t * (3.0f - 2.0f * t);
        return a + (b - a) * t;
    }

    public static float Smootherstep(float a, float b, float t)
    {
        t = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
        return a + (b - a) * t;
    }

    public static float Smootheststep(float a, float b, float t)
    {
        t = t * t * t * t * (t * (t * (t * -20.0f + 70.0f) - 84.0f) + 35.0f);
        return a + (b - a) * t;
    }

    public static float CatmullRom(float a, float b, float t, float p0, float p1)
    {
        t = 0.5f * ((2.0f * 0.0f) +
            (-p0 + 1.0f) * t +
            (2.0f * p0 - 5.0f * 0.0f + 4.0f * 1.0f - p1) * t * t +
            (-p0 + 3.0f * 0.0f - 3.0f * 1.0f + p1) * t * t * t);
        return a + (b - a) * t;
    }


    public class Quadratic
    {
        public static float In(float a, float b, float t)
        {
            return a + (b - a) * t * t;
        }

        public static float Out(float a, float b, float t)
        {
            return a - (b - a) * t * (t - 2.0f);
        }

        public static float InOut(float a, float b, float t)
        {
            t *= 2.0f;
            if (t < 1.0f)
                return a + (b - a) / 2.0f * t * t;
            t -= 1.0f;
            return a - (b - a) / 2.0f * (t * (t - 2.0f) - 1.0f);
        }
    };

    public class Cubic
    {
        public static float In(float a, float b, float t)
        {
            return a + (b - a) * t * t * t;
        }

        public static float Out(float a, float b, float t)
        {
            t -= 1.0f;
            return a + (b - a) * (t * t * t + 1.0f);
        }

        public static float InOut(float a, float b, float t)
        {
            t *= 2.0f;
            if (t < 1.0f)
                return a + (b - a) / 2.0f * t * t * t;
            t -= 2.0f;
            return a + (b - a) / 2.0f * (t * t * t + 2.0f);
        }
    };

    public class Quartic
    {
        public static float In(float a, float b, float t)
        {
            return a + (b - a) * t * t * t * t;
        }

        public static float Out(float a, float b, float t)
        {
            t -= 1.0f;
            return a - (b - a) * (t * t * t * t - 1.0f);
        }

        public static float InOut(float a, float b, float t)
        {
            t *= 2.0f;
            if (t < 1.0f)
                return a + (b - a) / 2.0f * t * t * t * t;
            t -= 2.0f;
            return a - (b - a) / 2.0f * (t * t * t * t - 2.0f);
        }
    };

    public class Quintic
    {
        public static float In(float a, float b, float t)
        {
            return a + (b - a) * t * t * t * t * t;
        }

        public static float Out(float a, float b, float t)
        {
            t -= 1.0f;
            return a + (b - a) * (t * t * t * t * t + 1.0f);
        }

        public static float InOut(float a, float b, float t)
        {
            t *= 2.0f;
            if (t < 1.0f)
                return a + (b - a) / 2.0f * t * t * t * t * t;
            t -= 2.0f;
            return a + (b - a) / 2.0f * (t * t * t * t * t + 2.0f);
        }
    };

    public class Sinusoidal
    {
        public static float In(float a, float b, float t)
        {
            return -(b - a) * Mathf.Cos(t * Mathf.PI / 2.0f) + b;
        }

        public static float Out(float a, float b, float t)
        {
            return a + (b - a) * Mathf.Sin(t * Mathf.PI / 2.0f);
        }

        public static float InOut(float a, float b, float t)
        {
            return a - (b - a) / 2.0f * (Mathf.Cos(Mathf.PI * t) - 1);
        }
    };

    public class Exponential
    {
        public static float In(float a, float b, float t)
        {
            return a + (b - a) * Mathf.Pow(2.0f, 10.0f * (t - 1.0f));
        }

        public static float Out(float a, float b, float t)
        {
            return a + (b - a) * (-Mathf.Pow(2.0f, -10.0f * t) + 1.0f);
        }

        public static float InOut(float a, float b, float t)
        {
            t *= 2.0f;
            if (t < 1.0f)
                return a + (b - a) / 2.0f * Mathf.Pow(2.0f, 10.0f * (t - 1.0f));
            t -= 1.0f;
            return a + (b - a) / 2.0f * (-Mathf.Pow(2.0f, -10.0f * t) + 2.0f);
        }
    };

    public class Circular
    {
        public static float In(float a, float b, float t)
        {
            return a - (b - a) * (Mathf.Sqrt(1.0f - t * t) - 1.0f);
        }

        public static float Out(float a, float b, float t)
        {
            t -= 1.0f;
            return a + (b - a) * Mathf.Sqrt(1.0f - t * t);
        }

        public static float InOut(float a, float b, float t)
        {
            t *= 2.0f;
            if (t < 1.0f)
                return a - (b - a) / 2.0f * (Mathf.Sqrt(1.0f - t * t) - 1.0f);
            t -= 2.0f;
            return a + (b - a) / 2.0f * (Mathf.Sqrt(1.0f - t * t) + 1.0f);
        }
    };


    public class Elastic
    {
        public static float In(float a, float b, float t)
        {
            if (t == 0)
                return a;
            if (t == 1)
                return b;
            return a + (b - a) * (-Mathf.Pow(2f, 10f * (t -= 1f)) * Mathf.Sin((t - 0.1f) * (2f * Mathf.PI) / 0.4f));
        }

        public static float Out(float a, float b, float t)
        {
            if (t == 0)
                return a;
            if (t == 1)
                return b;
            return a + (b - a) * (Mathf.Pow(2f, -10f * t) * Mathf.Sin((t - 0.1f) * (2f * Mathf.PI) / 0.4f) + 1f);
        }

        public static float InOut(float a, float b, float t)
        {
            if ((t *= 2f) < 1f)
                return a + (b - a) * (-0.5f * Mathf.Pow(2f, 10f * (t -= 1f)) * Mathf.Sin((t - 0.1f) * (2f * Mathf.PI) / 0.4f));
            return a + (b - a) * (Mathf.Pow(2f, -10f * (t -= 1f)) * Mathf.Sin((t - 0.1f) * (2f * Mathf.PI) / 0.4f) * 0.5f + 1f);
        }
    };

    public class Back
    {
        static float s = 1.70158f;
        static float s2 = 2.5949095f;

        public static float In(float a, float b, float t)
        {
            return a + (b - a) * (t * t * ((s + 1f) * t - s));
        }

        public static float Out(float a, float b, float t)
        {
            return a + (b - a) * ((t -= 1f) * t * ((s + 1f) * t + s) + 1f);
        }

        public static float InOut(float a, float b, float t)
        {
            if ((t *= 2f) < 1f)
                return a + (b - a) * (0.5f * (t * t * ((s2 + 1f) * t - s2)));
            return a + (b - a) * (0.5f * ((t -= 2f) * t * ((s2 + 1f) * t + s2) + 2f));
        }
    };

    public class Bounce
    {

        private static float In(float t)
        {
            return 1f - Out(1f - t);
        }

        private static float Out(float t)
        {
            if (t < (1f / 2.75f))
                return 7.5625f * t * t;
            else if (t < (2f / 2.75f))
                return 7.5625f * (t -= (1.5f / 2.75f)) * t + 0.75f;
            else if (t < (2.5f / 2.75f))
                return 7.5625f * (t -= (2.25f / 2.75f)) * t + 0.9375f;
            else
                return 7.5625f * (t -= (2.625f / 2.75f)) * t + 0.984375f;
        }

        private static float InOut(float t)
        {
            if (t < 0.5f)
                return In(t * 2f) * 0.5f;
            return Out(t * 2f - 1f) * 0.5f + 0.5f;
        }

        public static float In(float a, float b, float t)
        {
            return a + (b - a) * In(t);
        }

        public static float Out(float a, float b, float t)
        {
            return a + (b - a) * Out(t);
        }

        public static float InOut(float a, float b, float t)
        {
            return a + (b - a) * InOut(t);
        }
    };

}
