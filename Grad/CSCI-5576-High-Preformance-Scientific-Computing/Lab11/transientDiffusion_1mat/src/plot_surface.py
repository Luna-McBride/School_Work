import matplotlib.pyplot as plt
from matplotlib import cm
import matplotlib.colors as mcolors
import matplotlib.animation as animation
import numpy as np
import glob


def points_to_grid(pts: np.array) -> tuple[np.array]:
    """
    Turns a set of x,y,z points into 2D arrays
    X,Y,Z which are the x,y,z values at every
    point in a 2D grid.

    Args:
        points (np.array): npoints rows, 4 columns.
                           columns are in x, y, z order.
                           The fourth column is the PE
                           that this came from.

    Returns:
        X, Y, Z    (np.arrays) with ny rows, nx columns.
    """
    # Create a mapping from unique
    # x and y vals to columns/row numbers
    xvals = np.unique(pts[:, 0])
    yvals = np.unique(pts[:, 1])

    xindx = {}
    for i, x in enumerate(xvals):
        xindx[x] = i
    yindx = {}
    for i, y in enumerate(yvals):
        yindx[y] = i

    # Fill in the Z grid
    X, Y = np.meshgrid(xvals, yvals)
    Z = np.zeros_like(X)

    for pt in pts:
        ix = xindx[pt[0]]
        iy = yindx[pt[1]]
        Z[iy, ix] = pt[2]

    return X, Y, Z


def load_points(root: str, timestamp: str = "", return_fname: bool = False) -> list[np.array]:
    """
    Loads the points from all files with the given root

    Args:
        root (str): root of files to load
        timestamp (str): timestamp assumed to be write before .plt
                         for plotting a single timestep in a transient sim.
        return_fnames (bool): return a dictionary that maps filename to array
                              instead of just all the points.

    Returns:
        list[np.array]: list where each entry is an np.array
                        with columns x, y, z, and file number
    """

    dfiles = sorted(glob.glob(root))
    # Filter for .plt
    dfiles = [x for x in dfiles if ".plt" in x]

    # Filter for timestamp
    if timestamp != "":
        dfiles = [x for x in dfiles if f"{timestamp}.plt" in x]
    if return_fname:
        pts = {}
    else:
        pts = []
    for i, f in enumerate(dfiles):
        from_file = np.loadtxt(f)
        data_with_pe = np.hstack((from_file, i*np.ones((from_file.shape[0], 1))))
        if return_fname:
            pts[f] = data_with_pe
        else:
            pts.append(data_with_pe)

    return pts


def single_surface_plot(root: str, outfile: str, timestamp: str = "") -> plt.figure:
    """
    Generates a single surface plot of the files desired

    Args:
        root (str): files to grab to plot
        outfile (str): file to write figure to
        timestamp (str): timestamp assumed to be write before .plt
                         for plotting a single timestep in a transient sim.

    Returns:
        plt.figure: handle to figure object
    """

    # Load data
    points = load_points(root+"*", timestamp)

    fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    for i, pts in enumerate(points):
        X, Y, Z = points_to_grid(pts)
        surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, linewidth=1,
                               alpha=0.8, label=f"PE {i}")
        surf._edgecolors2d = surf._edgecolor3d
        surf._facecolors2d = surf._facecolor3d

    ax.legend()

    for pts in points:
        ax.scatter(pts[:, 0], pts[:, 1], pts[:, 2], marker=".", c="k", s=0.5,
                   alpha=0.25)

    if outfile != "":
        plt.savefig(outfile)

    return fig


def multi_surface_plot(flist: list[str], outfile: str, timestamp: str = "") -> plt.figure:
    """
    Generates a surface plot for each set of files

    Args:
        flist (str): files to grab to plot
        outfile (str): file to write figure to

    Returns:
        plt.figure: handle to figure object
    """

    # Make figure
    fig, ax = plt.subplots(ncols=len(flist), subplot_kw={"projection": "3d"})
    fig.set_size_inches(6*len(flist), 6)

    for i, root in enumerate(flist):
        # Load data
        points = load_points(root+"*")

        ax[i].set_xlabel("X")
        ax[i].set_ylabel("Y")
        ax[i].set_zlabel("Z")
        for iPE, pts in enumerate(points):
            X, Y, Z = points_to_grid(pts)
            surf = ax[i].plot_surface(X, Y, Z, rstride=1, cstride=1, linewidth=1,
                                      alpha=0.8, label=f"PE {iPE}")
            surf._edgecolors2d = surf._edgecolor3d
            surf._facecolors2d = surf._facecolor3d

        # ax[i].legend()
        ax[i].set_title(root)

        for pts in points:
            ax[i].scatter(pts[:, 0], pts[:, 1], pts[:, 2], marker=".", c="k",
                          s=0.5, alpha=0.25)

    if outfile != "":
        plt.savefig(outfile)

    return fig


def double_surface_plot(root: str, root2: str, outfile: str, timestamp: str = "") -> plt.figure:
    """
    Generates a surface plot comparing two surfaces.
    Plots them both in the same axes, and shows a residual
    as well.

    Args:
        root (str): files to grab to plot
        root2 (str): other set of files to grab to plot
        outfile (str): file to write figure to
        timestamp (str): timestamp assumed to be write before .plt
                         for plotting a single timestep in a transient sim.

    Returns:
        plt.figure: handle to figure object
    """

    # Load data
    points = np.concatenate(load_points(root+"*", timestamp))
    points2 = np.concatenate(load_points(root2+"*", timestamp))

    fig = plt.figure(figsize=(12,6))
    ax = fig.add_subplot(1, 2, 1, projection='3d')

    # Plot surfaces
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    labels = [root, root2]
    Zvals = []
    for i, pts in enumerate([points, points2]):
        X, Y, Z = points_to_grid(pts)
        Zvals.append(Z)
        surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1,
                               linewidth=1, alpha=0.5, label=labels[i])
        surf._edgecolors2d = surf._edgecolor3d
        surf._facecolors2d = surf._facecolor3d

    ax.legend()

    ax = fig.add_subplot(1, 2, 2)

    # Plot residual

    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_title("Difference Between Surface 1 and 2")
    cmesh = ax.pcolormesh(X, Y, np.abs(Zvals[1] - Zvals[0]))
    plt.colorbar(cmesh)

    plt.tight_layout()

    if outfile != "":
        plt.savefig(outfile)

    return fig


def make_video(root: str, outfile: str = "") -> plt.Figure:
    """
    Generates a video from the plt files gathered by root

    Args:
        root (str): captures all files that start with this
                    string and ends in .plt.
        outfile (str): file to save out as. If none is given
                       opens an interactive plot. Should be
                       a .mp4 or .gif.

    Returns:
        plt.Figure: matplotlib figure
    """

    # Load data
    points = load_points(root+"*", return_fname=True)

    # Map timestamp to data
    by_timestamp = {}
    for f in points:
        i1 = f[::-1].find(".")
        i2 = f[::-1].find("_")
        t = int(f[::-1][i1+1:i2][::-1])
        if t in by_timestamp:
            by_timestamp[t].append(points[f])
        else:
            by_timestamp[t] = [points[f]]

    fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
    artists = []
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    max_PE = max([len(by_timestamp[x]) for x in by_timestamp])
    colors = list(mcolors.TABLEAU_COLORS.keys())[:max_PE]
    for t in sorted(by_timestamp.keys()):
        points = by_timestamp[t]
        container = []
        for i, pts in enumerate(points):
            X, Y, Z = points_to_grid(pts)
            surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, linewidth=1,
                                   alpha=0.8, label=f"PE {i}", color=colors[i])
            surf._edgecolors2d = surf._edgecolor3d
            surf._facecolors2d = surf._facecolor3d
            container += [surf]

        container += [ax.legend()]
        container += [ax.text(0,0, 1.25, s=str(t), bbox={'facecolor':'w', 'alpha':0.5, 'pad':5},
                                    transform=ax.transAxes, ha="center")]

        for pts in points:
            container += [ax.scatter(pts[:, 0], pts[:, 1], pts[:, 2],
                                     marker=".", c="k", s=0.5, alpha=0.25)]
        artists.append(container)

    ani = animation.ArtistAnimation(fig=fig, artists=artists, interval=100)

    try:
        writer = animation.FFMpegWriter(fps=30)
        ani.save(filename=outfile, writer=writer)
    except:
        print("ffmpeg writer not found or filetype not supported (try .mp4), trying a gif with Pillow")
        try:
            writer = animation.PillowWriter(fps=30)
            ani.save(filename=outfile.replace(".mp4", ".gif"), writer=writer)
        except:
            print("Pillow writer didn't work either :(")

    return fig


if __name__ == "__main__":

    # Simple command line interface
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--file_root", type=str, default="phi_",
                        help="Surface files to plot, takes all files starting with this.")
    parser.add_argument("-f2", "--file_root2", type=str, default="",
                        help="Second set of files to plot in a comparison, takes all files starting with this.")
    parser.add_argument("-flist", "--file_list", type=str, default="",
                        help="list of file roots")
    parser.add_argument("-o", "--output", type=str, default="",
                        help="File to save figure to.")
    parser.add_argument("-t", "--timestamp", type=str, default="")
    parser.add_argument("-v", "--video", type=str, default="")

    args = parser.parse_args()

    print("Arguments: ", args)

    if args.video == "1":
        fig = make_video(args.file_root, args.output)

    elif args.file_root2 == "" and args.file_list == "" and args.video != "1":
        fig = single_surface_plot(args.file_root, args.output, args.timestamp)

    elif args.file_list == "" and args.video != "1":
        fig = double_surface_plot(args.file_root, args.file_root2, args.output, args.timestamp)

    else:
        flist = args.file_list.split(",")
        print(flist)
        fig = multi_surface_plot(flist, args.output, args.timestamp)

    if args.output == "":
        plt.show()
